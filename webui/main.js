// SAAD MAHMOOD

import * as THREE from 'three';
import {GLTFLoader} from 'three/examples/jsm/loaders/GLTFLoader';
import {RGBELoader} from 'three/examples/jsm/loaders/RGBELoader.js';

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer({antialias: true, alpha: false});
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.setPixelRatio(window.devicePixelRatio);

// PMREM Generator for HDR environment
const pmremGenerator = new THREE.PMREMGenerator(renderer);
pmremGenerator.compileEquirectangularShader();

let envMap; // Define this outside to be accessible in the model load function

new RGBELoader()
    .setDataType(THREE.FloatType) // Set the data type to FloatType for HDR texture
    .load('./graphics/polly/bg_rs.hdr', function (hdrTexture) {
        hdrTexture.mapping = THREE.EquirectangularReflectionMapping;
        envMap = pmremGenerator.fromEquirectangular(hdrTexture).texture; // Create the envMap
        scene.environment = envMap; // Set it as the environment of the scene
        pmremGenerator.dispose();
        hdrTexture.dispose();
        updateMaterials(); // Update materials with the new envMap
    });

// Function to update materials with the environment map
function updateMaterials() {
    scene.traverse((object) => {
        if (object.material && 'envMap' in object.material) {
            object.material.envMap = envMap;
            object.material.needsUpdate = true;
        }
    });
}

const light = new THREE.AmbientLight(0xffffff, 5);
scene.add(light);

const gridHelper = new THREE.GridHelper(10, 10);
scene.add(gridHelper);

const sceneContainer = document.getElementById('scene-container');
sceneContainer.appendChild(renderer.domElement);

const loader = new GLTFLoader();
let mixer;
const clock = new THREE.Clock();
let currentAction; // Variable to keep track of the current action
let animations; //stores glTF animations

// Function to change the animation clip
function changeAnimationClip(emotion) {
    const clipName = emotionToClipName(emotion); // Map emotion to clip name
    const clip = animations ? THREE.AnimationClip.findByName(animations, clipName) : null;
    if (clip && mixer) {
        if (currentAction) {
            currentAction.stop(); // Stop current action
        }
        currentAction = mixer.clipAction(clip);
        currentAction.play();
    } else {
        console.error('Animation clip not found for', emotion);
    }
}

function emotionToClipName(emotion) {
    // Mapping of emotions to animation clip names
    const mapping = {
        "sadness": "sad",
        "anger": "mad",
        "love": "love",
        "surprise": "surprise",
        "joy": "joy",
        "fear": "scared",
        "default": "breathing",
        "thinking": "thinking"
    };
    return mapping[emotion] || mapping["default"];
}

// WebSocket connection to the Flask server
//const socket = io('http://localhost:5000', { transports: ['websocket'], upgrade: false });

// Listen for emotion updates from the server
//socket.on('emotion_update', (data) => {
//    changeAnimationClip(data.emotion);
//});

loader.load(
    './graphics/polly/testExport1.gltf',
    (gltf) => {
        console.log('Model loaded', gltf);
        const model = gltf.scene;
        model.scale.set(1.3, 1.3, 1.3);
        model.position.set(0, 0, 0);
        scene.add(model);

        mixer = new THREE.AnimationMixer(model);
        animations = gltf.animations; // Store the animations for later use

        // Start with the default "breathing" animation
        changeAnimationClip("default");

        animate(); // Start the animation loop here
    },
    (xhr) => {
        console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
    },
    (error) => {
        console.log('An error happened', error);
    }
);

// WebSocket connection to the Flask server
//const socket = io('http://localhost:5000');

// Listen for emotion updates from the server
//socket.on('emotion_update', (data) => {
//    changeAnimationClip(data.emotion);
//});

const socket = new WebSocket('ws://localhost:8080/ws');

socket.onopen = function (e) {
    console.log("Connection established!");
};

socket.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (data.action && data.action == "thinking") {
        changeAnimationClip("thinking");
        document.getElementById('output').textContent = data.thinkingText;
    } else if (data.response) {
        document.getElementById('output').textContent = data.response;
        if (data.emotion) {
            changeAnimationClip(data.emotion);
        }
    }
};

socket.onclose = function (event) {
    if (event.wasClean) {
        console.log(`Connection closed cleanly, code=${event.code}, reason=${event.reason}`);
    } else {
        // e.g. server process killed or network down
        console.log('Connection died');
    }
};

socket.onerror = function (error) {
    console.log(`[error] ${error.message}`);
};

camera.position.set(0, 5, 10);
camera.lookAt(scene.position);

function animate() {
    requestAnimationFrame(animate);
    const delta = clock.getDelta();

    if (mixer) {
        mixer.update(delta);
    }

    renderer.render(scene, camera);
}

window.addEventListener('resize', onWindowResize, false);

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}

