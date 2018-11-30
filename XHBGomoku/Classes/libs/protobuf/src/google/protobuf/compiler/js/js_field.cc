//
//  js_field.cpp
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <google/protobuf/compiler/js/js_field.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/compiler/js/js_primitive_field.h>
#include <google/protobuf/compiler/js/js_enum_field.h>
#include <google/protobuf/compiler/js/js_message_field.h>
#include <google/protobuf/compiler/js/js_string_field.h>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

FieldGenerator::~FieldGenerator() {}
    
void FieldGenerator::GenerateParsingCodeFromPacked(io::Printer* printer) const
{
    // Reaching here indicates a bug. Cases are:
    //   - This FieldGenerator should support packing, but this method should be
    //     overridden.
    //   - This FieldGenerator doesn't support packing, and this method should
    //     never have been called.
    GOOGLE_LOG(FATAL) << "GenerateParsingCodeFromPacked() "
    << "called on field generator that does not support packing.";
}
    
FieldGeneratorMap::FieldGeneratorMap(const Descriptor* descriptor)
    : descriptor_(descriptor),
    field_generators_(new scoped_ptr<FieldGenerator>[descriptor->field_count()])
{
    for (int i = 0; i < descriptor->field_count(); i++) {
        FieldGenerator* generator = MakeGenerator(descriptor->field(i));
        field_generators_[i].reset(generator);
    }
    
}

FieldGeneratorMap::~FieldGeneratorMap()
{
    
}

const FieldGenerator& FieldGeneratorMap::get(const FieldDescriptor* field) const
{
    return *field_generators_[field->index()];
}
    
FieldGenerator* FieldGeneratorMap::MakeGenerator(const FieldDescriptor* field)
{
    if (field->is_repeated()) {
        switch (GetJSType(field)) {
            case JSTYPE_MESSAGE:
                return new RepeatedMessageFieldGenerator(field);
            case JSTYPE_STRING:
                return new RepeatedStringFieldGenerator(field);
            case JSTYPE_ENUM:
                return new RepeatedEnumFieldGenerator(field);
            default:
                return new RepeatedPrimitiveFieldGenerator(field);
        }
    } else {
        switch (GetJSType(field)) {
            case JSTYPE_MESSAGE:
                return new MessageFieldGenerator(field);
            case JSTYPE_STRING:
                return new StringFieldGenerator(field);
            case JSTYPE_ENUM:
                return new EnumFieldGenerator(field);
            default:
                return new PrimitiveFieldGenerator(field);
        }
    }
}
    
}
}
}
}
