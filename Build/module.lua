function CreateConnectProject(basePath, coreBasePath)
    project ("Connect")
        kind ("StaticLib")
        language ("C++")

        includedirs
        {
            basePath .. "/Code/connect/include/",
            coreBasePath .. "/Code/core/include/",
            basePath .. "/ThirdParty/GameNetworkingSockets/include/",
        }

        files
        {
            basePath .. "/Code/connect/include/**.hpp",
            basePath .. "/Code/connect/src/**.cpp",
        }
        
        defines { "STEAMNETWORKINGSOCKETS_STATIC_LINK" }
        
        filter { "action:gmake*", "language:C++" }
            defines
            {
                'POSIX',
                'LINUX',
                'GNUC',
                'GNU_COMPILER',
            }

        filter ""
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
            
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/any_lite.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/arena.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/extension_set.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/generated_message_table_driven_lite.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/generated_message_util.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/implicit_weak_message.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/coded_stream.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/strtod.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/io_win32.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/zero_copy_stream.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/io/zero_copy_stream_impl_lite.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/message_lite.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/parse_context.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/repeated_field.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/bytestream.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/common.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/int128.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/status.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/statusor.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/stringpiece.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/stringprintf.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/structurally_valid.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/strutil.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/stubs/time.cc",
            basePath .. "/ThirdParty/protobuf/src/google/protobuf/wire_format_lite.cc"


        }
        
        defines
        {
            "HAVE_PTHREAD"
        }

        filter { "action:gmake*", "language:C++" }
            buildoptions { "-fPIC" }
            linkoptions ("-fPIC")

        filter ""
end

function CreateSteamNetProject(basePath)
    project ("SteamNet")
        kind ("StaticLib")
        language ("C++")
        
        characterset "ASCII"

        includedirs
        {
            basePath .. "/ThirdParty/protobuf/src/",
            basePath .. "/ThirdParty/openssl/include/",
            basePath .. "/ThirdParty/GameNetworkingSockets/include/",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/",
            basePath .. "/ThirdParty/SteamCodegen/",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier0",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/vstdlib",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/public",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/external/curve25519-donna",
            basePath .. "/ThirdParty/"
        }

        files
        {
            basePath .. "/ThirdParty/GameNetworkingSockets/include/**/*.h",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/**/*.h",
            basePath .. "/ThirdParty/gnsconfig.h",
            
            basePath .. "/ThirdParty/SteamCodegen/*.h",
            basePath .. "/ThirdParty/SteamCodegen/*.cc",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/crypto_25519_donna.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/crypto_25519_openssl.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/crypto_openssl.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/crypto_textencode.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/keypair.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/steamid.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/common/opensslwrapper.cpp",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/vstdlib/strtools.cpp",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier0/dbg.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier0/platformtime.cpp",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1/bitstring.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1/ipv6text.c",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1/netadr.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1/utlbuffer.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/tier1/utlmemory.cpp",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/external/curve25519-donna/curve25519.c",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/external/curve25519-donna/curve25519_VALVE_sse2.c",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/external/ed25519-donna/ed25519_VALVE.c",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/external/ed25519-donna/ed25519_VALVE_sse2.c",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/steamnetworkingsockets_certs.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/steamnetworkingsockets_certstore.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/steamnetworkingsockets_connections.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/steamnetworkingsockets_flat.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/steamnetworkingsockets_lowlevel.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/steamnetworkingsockets_shared.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/steamnetworkingsockets_stats.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/steamnetworkingsockets_snp.cpp",
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/steamnetworkingsockets_udp.cpp",
            
            basePath .. "/ThirdParty/GameNetworkingSockets/src/steamnetworkingsockets/clientlib/csteamnetworkingsockets.cpp",

        }
        
        defines
        {
            "STEAMNETWORKINGSOCKETS_STATIC_LINK",
            "ENABLE_OPENSSLCONNECTION",
            "CRYPTO_DISABLE_ENCRYPT_WITH_PASSWORD",
            "GOOGLE_PROTOBUF_NO_RTTI",
            "VALVE_CRYPTO_25519_DONNA",
            "STEAMNETWORKINGSOCKETS_CRYPTO_VALVEOPENSSL"
        }
        
        links
        {
            "protobuf",
        }
        
        filter { "architecture:*86" }
            libdirs { basePath .. "/ThirdParty/openssl/lib/x32" }

        filter { "architecture:*64" }
            libdirs { basePath .. "/ThirdParty/openssl/lib/x64" }

        filter { "action:vs*" }
            links
            {
                "libcryptoMT.lib",
                "libsslMT.lib",
                "ws2_32",
                "Crypt32.lib"
            }
        
        filter { "action:gmake*", "language:C++" }
            defines
            {
                'POSIX',
                'LINUX',
                'GNUC',
                'GNU_COMPILER',
            }
            
            links
            {
                "ssl",
                "crypto"
            }

        filter ""
end
