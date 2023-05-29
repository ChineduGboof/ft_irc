FROM ubuntu:latest

# Install dependencies
RUN apt-get update && \
    apt-get install -y irssi

# Expose port 6667
EXPOSE 6667

# Set the default command to run Irssi
CMD ["irssi"]
