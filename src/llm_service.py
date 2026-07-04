# we need to autostart this for LLMProcessing.cpp to work properly
#SAAD MAHMOOD

import flask
from flask import request, jsonify
from transformers import AutoTokenizer, GPT2LMHeadModel, pipeline

app = flask.Flask(__name__)
# Initialize the LLM (tokenizer + model)tokenizer
tokenizer = AutoTokenizer.from_pretrained("gpt2")
model = GPT2LMHeadModel.from_pretrained("gpt2", pad_token_id=tokenizer.eos_token_id)

# Initialize the sentiment analysis model
emotion_pipeline = pipeline("sentiment-analysis", model="esuriddick/distilbert-base-uncased-finetuned-emotion")

import logging

# Setup logging
logging.basicConfig(level=logging.DEBUG)

@app.route('/generate', methods=['POST'])
def generate_text():
    print("made it here")
    try:
        content = request.json
        #logging.debug(f"Received content: {content}")
        input_text = content['text']
        input_ids = tokenizer.encode(input_text, return_tensors='pt')
        beam_output = model.generate(
            input_ids,
            max_length=content.get('max_length', 60),
            num_beams=content.get('num_beams', 5),
            no_repeat_ngram_size=content.get('no_repeat_ngram_size', 3),
            temperature=content.get('temperature', 0.7),
            top_k=content.get('top_k', 50),
            early_stopping=True
        )
        output = tokenizer.decode(beam_output[0], skip_special_tokens=True)
        # Remove the repeated prompt part from the output
        output = output.replace(input_text, '', 1).strip()

        print(output)
        #logging.debug(f"Generated output: {output}")
        return jsonify({"result": output})
    except Exception as e:
        #logging.error(f"An error occurred: {e}")
        return jsonify({"error": str(e)}), 500

@app.route('/generate_emotion', methods=['POST'])
def generate_emotion():
    print("made it here2")
    try:
        content = request.json
        input_text = content['text']
        # Perform sentiment analysis
        results = emotion_pipeline(input_text)
        return jsonify({"result": results})
    except Exception as e:
        logging.error(f"An error occurred: {e}")
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True, port=5000)



#https://huggingface.co/esuriddick/distilbert-base-uncased-finetuned-emotion
#using this model for sentiment analysis, it was trained on this dataset: https://huggingface.co/datasets/dair-ai/emotion
