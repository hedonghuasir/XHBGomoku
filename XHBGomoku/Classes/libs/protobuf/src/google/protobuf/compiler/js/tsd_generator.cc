//
//  js_generator.cpp
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <google/protobuf/compiler/js/tsd_generator.h>
#include <google/protobuf/compiler/js/js_file.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {
    
TsdGenerator::TsdGenerator(){}
TsdGenerator::~TsdGenerator(){}
    
bool TsdGenerator::Generate(const FileDescriptor* file,
              const string& parameter,
              GeneratorContext* generator_context,
              string* error) const
{
    vector<pair<string, string> > options;
    ParseGeneratorParameter(parameter, &options);
    
    // TODO options
    
    string basename = StripProto(file->name());
    basename.append(".pb");
    
    FileGenerator file_generator(file);
    
    string package_dir = PackageToDir(file_generator.file_package());
    
    {
        scoped_ptr<io::ZeroCopyOutputStream> output(
                                                    generator_context->Open(package_dir + basename + ".d.ts"));
        io::Printer printer(output.get(), '$');
        file_generator.GenerateTSDSource(&printer);
    }
    
    return true;
}
    
}
}
}
}
