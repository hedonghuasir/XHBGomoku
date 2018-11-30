//
//  js_generator.h
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_GENERATOR_H__

#include <string>
#include <google/protobuf/compiler/code_generator.h>

#include <stdio.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {
    
// CodeGenerator implementation which generates a js source file.
// If you create your own protocol compiler binary and you want
// it to support js output, you can do so by registering an instance of this
// CodeGenerator with the CommandLineInterface in your main() function.
class LIBPROTOC_EXPORT JsGenerator : public CodeGenerator {
public:
    JsGenerator();
    ~JsGenerator();
    
    // implements CodeGenerator ----------------------------------------
    bool Generate(const FileDescriptor* file,
                  const string& parameter,
                  GeneratorContext* generator_context,
                  string* error) const;
    
private:
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(JsGenerator);
};

}
}
}
}

#endif /* defined(__protobuf__js_generator__) */
