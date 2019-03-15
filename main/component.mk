#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

# embed files from the "certs" directory as binary data symbols
# in the app
COMPONENT_EMBED_FILES := html/index.html html/avionics.js html/avionics.js.gz html/favicon.ico
