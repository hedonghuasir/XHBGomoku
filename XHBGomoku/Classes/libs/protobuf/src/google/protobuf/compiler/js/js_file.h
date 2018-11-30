//
//  js_file.h
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_FILE_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_FILE_H__

#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {
class FileDescriptor;        // descriptor.h
namespace io {
    class Printer;             // printer.h
}
}

namespace protobuf {
namespace compiler {
namespace js {
    
class EnumGenerator;           // enum.h
class MessageGenerator;        // message.h
    
class FileGenerator {
public:
    // See generator.cc for the meaning of dllexport_decl.
    explicit FileGenerator(const FileDescriptor* file);
    ~FileGenerator();
    
    void GenerateSource(io::Printer* printer);
    void GenerateTSDSource(io::Printer* printer);
    
    
    const string& file_package() {return file_package_;}
private:
    
    const FileDescriptor* file_;
    
    scoped_array<scoped_ptr<MessageGenerator> > message_generators_;
    scoped_array<scoped_ptr<EnumGenerator> > enum_generators_;
//    scoped_array<scoped_ptr<ServiceGenerator> > service_generators_;
//    scoped_array<scoped_ptr<ExtensionGenerator> > extension_generators_;
    
    string file_package_;
    
//    const Options options_;
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
};
    
}
}
}
}

#endif /* defined(__protobuf__js_file__) */
