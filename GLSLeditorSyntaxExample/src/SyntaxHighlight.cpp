//
//  SyntaxHighlight.cpp
//  ofxGLEditor_example
//
//  Created by Joshua Batty on 26/03/2015.
//
//

#include "SyntaxHighlight.h"

void SyntaxHighlight::setup(ofxEditorColorScheme *colorScheme){
    
    setDataTypes();
    setPreprocessor();
    setFunctions();
    
    // setup glsl data types and functinon
    // DATA TYPES
    for (int i = 0; i < dataType.size(); i++) {
        colorScheme->setWordColor(dataType[i], ofColor(246,70,185));
        colorScheme->setWordColor(dataType[i]+"(", ofColor(246,70,185));
    }
    
    // PREPROCESSOR
    for (int i = 0; i < preprocessor.size(); i++) {
        colorScheme->setWordColor(preprocessor[i], ofColor(230,121,57));
    }
    
    // FUNCTIONS
    for (int i = 0; i < functions.size(); i++) {
        colorScheme->setWordColor(functions[i], ofColor(11,179,244));
        colorScheme->setWordColor(functions[i]+"(", ofColor(11,179,244));
    }
    
    colorScheme->setSingleLineComment("//");
    colorScheme->setMultiLineComment("/*", "*/");
    
    // syntax highlighter colors
    colorScheme->setStringColor(ofColor::green);
    colorScheme->setNumberColor(ofColor::white);
    colorScheme->setCommentColor(ofColor::green);
}

void SyntaxHighlight::setDataTypes(){
    
    // Qualifiers
        dataType.push_back("void");
        dataType.push_back("uniform");
        dataType.push_back("const");
        dataType.push_back("in");
        dataType.push_back("out");
        dataType.push_back("inout");
    
    // Scalars
        dataType.push_back("int");
        dataType.push_back("float");
        dataType.push_back("uint");
        dataType.push_back("bool");
    
    // Vectors
        dataType.push_back("vec2");
        dataType.push_back("vec3");
        dataType.push_back("vec4");
        dataType.push_back("ivec2");
        dataType.push_back("ivec3");
        dataType.push_back("ivec4");
        dataType.push_back("uvec2");
        dataType.push_back("uvec3");
        dataType.push_back("uvec4");
        dataType.push_back("bvec2");
        dataType.push_back("bvec3");
        dataType.push_back("bvec4");
    
    // Matrices
        dataType.push_back("mat2");
        dataType.push_back("mat3");
        dataType.push_back("mat4");
    
    // Samplers
        dataType.push_back("sampler1D");
        dataType.push_back("sampler2D");
        dataType.push_back("sampler3D");
        dataType.push_back("samplerCube");
        dataType.push_back("sampler2DRect");
        dataType.push_back("sampler1DShadow");
        dataType.push_back("sampler2DShadow");
        dataType.push_back("sampler2DRectShadow");
        dataType.push_back("sampler1DArray");
        dataType.push_back("sampler2DArray");
        dataType.push_back("sampler1DArrayShadow");
        dataType.push_back("sampler2DArrayShadow");
        dataType.push_back("isampler1D");
        dataType.push_back("isampler2D");
        dataType.push_back("isampler3D");
        dataType.push_back("isamplerCube");
        dataType.push_back("isampler2DRect");
        dataType.push_back("isampler2DArray");
        dataType.push_back("isamplerBuffer");
        dataType.push_back("usampler1D");
        dataType.push_back("usampler2D");
        dataType.push_back("usampler3D");
        dataType.push_back("usamplerCube");
        dataType.push_back("usampler2DRect");
        dataType.push_back("usampler1DArray");
        dataType.push_back("usampler2DArray");
}

void SyntaxHighlight::setPreprocessor(){
        preprocessor.push_back("#define");
        preprocessor.push_back("#undef");
        preprocessor.push_back("#if");
        preprocessor.push_back("#ifdef");
        preprocessor.push_back("#ifndef");
        preprocessor.push_back("#else");
        preprocessor.push_back("#elif");
        preprocessor.push_back("#endif");
        preprocessor.push_back("#error");
        preprocessor.push_back("#line");
        preprocessor.push_back("#pragma");
        preprocessor.push_back("#version");
        preprocessor.push_back("#extension");
}

void SyntaxHighlight::setFunctions(){
    
    // Angle and Trigonometry Functions
        functions.push_back("radians");
        functions.push_back("degrees");
        functions.push_back("sin");
        functions.push_back("cos");
        functions.push_back("tan");
        functions.push_back("asin");
        functions.push_back("acos");
        functions.push_back("atan");
        functions.push_back("sinh");
        functions.push_back("cosh");
        functions.push_back("tanh");
        functions.push_back("asinh");
        functions.push_back("acosh");
        functions.push_back("atanh");
        
    // Exponential Functions
        functions.push_back("pow");
        functions.push_back("exp");
        functions.push_back("log");
        functions.push_back("exp2");
        functions.push_back("log2");
        functions.push_back("sqrt");
        functions.push_back("inversesqrt");
        
    // Common Functions
        functions.push_back("abs");
        functions.push_back("sign");
        functions.push_back("floor");
        functions.push_back("trunc");
        functions.push_back("round");
        functions.push_back("roundEven");
        functions.push_back("cell");
        functions.push_back("fract");
        functions.push_back("mod");
        functions.push_back("min");
        functions.push_back("maxclamp");
        functions.push_back("mix");
        functions.push_back("step");
        functions.push_back("smoothstep");
        functions.push_back("isnan");
        functions.push_back("isinf");
        
    // Geometric Functions
        functions.push_back("length");
        functions.push_back("distance");
        functions.push_back("dot");
        functions.push_back("cross");
        functions.push_back("normalize");
        functions.push_back("faceforward");
        functions.push_back("reflect");
        functions.push_back("refract");
        
    // Matrix Functions
        functions.push_back("matrixCompMult");
        functions.push_back("transpose");
        functions.push_back("inverse");
        functions.push_back("outerProduct");

    // Vector Relational Functions
        functions.push_back("lessThan");
        functions.push_back("lessThanEqual");
        functions.push_back("greaterThan");
        functions.push_back("greaterThanEqual");
        functions.push_back("equal");
        functions.push_back("notEqual");
        functions.push_back("any");
        functions.push_back("all");
        functions.push_back("not");
        
    // Texture Access Functions
        functions.push_back("textureSize");
        functions.push_back("texture");
        functions.push_back("textureProj");
        functions.push_back("textureLod");
        functions.push_back("textureGrad");
        functions.push_back("textureOffset");
        functions.push_back("texelFetch");
        functions.push_back("texelFetchOffset");
        functions.push_back("textureProjLod");
        functions.push_back("textureProjGrad");
        functions.push_back("textureProjOffset");
        functions.push_back("textureLodOffset");
        functions.push_back("textureGradOffset");
        functions.push_back("textureProjLodOffset");
        functions.push_back("textureProjGradOffset");

    // Fragment Processing Functions
        functions.push_back("dFdx");
        functions.push_back("dFdy");
        functions.push_back("fwidth");
        
    // Noise Functions
        functions.push_back("noise1");
        functions.push_back("noise2");
        functions.push_back("noise3");
        functions.push_back("noise4");
}