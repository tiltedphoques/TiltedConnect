function CreateConnectProject(basePath, coreBasePath)
    project ("Connect")
        kind ("StaticLib")
        language ("C++")

        includedirs
        {
            basePath .. "/Code/connect/include/",
            coreBasePath .. "/Code/core/include/",
        }

        files
        {
            basePath .. "/Code/connect/include/**.h",
            basePath .. "/Code/connect/src/**.cpp",
        }
end

function CreateProtobufProject(basePath)
    project ("protobuf")
        kind ("StaticLib")
        language ("C++")

        includedirs
        {
            basePath .. "/ThirdParty/protobuf/src/"
        }

        files
        {
            basePath .. "/ThirdParty/protobuf/src/**/*.h",
            basePath .. "/ThirdParty/protobuf/src/**/*.inc",

            basePath .. "/ThirdParty/protobuf/src/google/protobuf/any.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/any.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/api.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/compiler/importer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/compiler/parser.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/descriptor.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/descriptor.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/descriptor_database.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/duration.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/dynamic_message.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/empty.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/extension_set_heavy.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/field_mask.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/generated_message_reflection.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/generated_message_table_driven.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/gzip_stream.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/printer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/tokenizer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/zero_copy_stream_impl.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/map_field.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/message.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/reflection_ops.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/service.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/source_context.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/struct.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/mathlimits.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/substitute.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/text_format.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/timestamp.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/type.pb.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/unknown_field_set.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/delimited_message_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/field_comparator.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/field_mask_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/datapiece.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/default_value_objectwriter.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/error_listener.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/field_mask_utility.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/json_escaping.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/json_objectwriter.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/json_stream_parser.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/object_writer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/proto_writer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/protostream_objectsource.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/protostream_objectwriter.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/type_info.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/type_info_test_helper.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/internal/utility.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/json_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/message_differencer.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/time_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/util/type_resolver_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/wire_format.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/wrappers.pb.cc",

        }
        
        defines
        {
            "HAVE_PTHREAD"
        }
end