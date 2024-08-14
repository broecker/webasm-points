all:
	emcc -o index.html -lGL -s MAX_WEBGL_VERSION=2 -s GL_ASSERTIONS main.cc
