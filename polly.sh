#!/bin/bash

# Start Python backend server
echo "Starting Python backend server..."
python3 src/llm_service.py &
BACKEND_PID=$!

# Wait for the Python server to start
echo "Waiting for Python server to start..."
sleep 5  # Adjust this as needed

# Open the Python service in the default browser
echo "Opening Python service in the default browser..."
xdg-open http://localhost:5000/  # Adjust the port if necessary

# Build and run the C++ application
echo "Building and running C++ application..."
mkdir -p cmake/build
cd cmake/build
cmake ../..
make
./Polly &
POLLY_PID=$!
cd ../../

# Start Vite for the front-end
echo "Starting Vite server for front-end..."
(cd webui && npx vite) &
VITE_PID=$!

# Wait for a few seconds to ensure Vite server is up and running
echo "Waiting for Vite server to start..."
sleep 5

# Open the web application in the default browser
echo "Opening web application in the default browser..."
xdg-open http://localhost:5173/ &

# Instructions for stopping the servers
echo "Servers are running. You can access the web application at http://localhost:5173/"
echo "To stop all servers, run: kill $BACKEND_PID $POLLY_PID $VITE_PID"

# Keep script running until all servers are stopped
wait $BACKEND_PID $POLLY_PID $VITE_PID
