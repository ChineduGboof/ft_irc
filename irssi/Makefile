.PHONY: all build run clean

# Build and run the Docker container
all: build run

# Build the Docker image
build:
	docker build -t irssi .

# Run the Irssi container
run:
	docker run -it irssi

# Clean up the Docker image
clean:
	docker image rm irssi
