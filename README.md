lhmodel
=======
LHModel for basic C++ value classes supporting a visitor pattern with a swagger-codegen client for generating LHModel C++ classes


Dockerfiles
-----------
1) Dockerfile.swagger-codegen - generate/buid/run the LHModelServerCodegen
2) Dockerfile.lhmodel         - builds liblhmodel

Codegen Usage
=============
See Dockerfile.swagger-codegen for an example of how to generate source files from a swagger specification. It supports all standard swagger options as well as three custom options:
1) modelIncludeDir - the expected directory where model header files will be placed, results in all model includes in the generated files to prefix the header file with "<modelIncludeDir>/"
2) namespace - the namespace to generate models into
3) customFormatTypesFile - a path to a json file which allows for existing swagger types and formats to be overridden and additional types/formats to be defined. The format file should consist of a json array of objects containing the following keys:
    - swaggerType - the swagger type
    - format - the swagger format
    - cpptype - the C++ type which the swagger type and swagger format pair should map to
    - includeFile - the include file containing the C++ type (can be null or excluded)
  
  liblhmodel Usage
  ================
  See examples/include/lhmodel_examples/lhmodel_examples/lhmodel_rapidjson.h and the corresponding tests for how to implement a visitor which can iterate over and mutate any model
