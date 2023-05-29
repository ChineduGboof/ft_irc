.PHONY: all build run clean

# Build and run the Docker container
all: build run

# Build the Docker image
build:
	docker build -t irssi .

# Run the Irssi container with port mapping
run:
	docker run -it -p 6667:6667 irssi

# Clean up the Docker image
clean:
	docker image rm irssi
