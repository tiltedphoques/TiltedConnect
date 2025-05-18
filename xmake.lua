set_languages("cxx20")

set_xmakever("2.5.1")

-- direct dependency version pinning
add_requires("tiltedcore v0.2.7", "hopscotch-map v2.3.1", "snappy 1.1.10", "gamenetworkingsockets v1.4.1", "catch2 2.13.9", "libuv v1.48.0")

-- dependencies' dependencies version pinning
add_requireconfs("*.mimalloc", { version = "2.1.7", override = true })
add_requireconfs("*.openssl", { version = "1.1.1-w", override = true })
add_requireconfs("*.cmake", { version = "3.30.2", override = true })
add_requireconfs("*.protobuf*", { version = "26.1", override = true }) 
add_requireconfs("**.abseil*", { version = "20250127.1", override = true }) 

add_rules("mode.debug","mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

if is_mode("release") then
    add_defines("NDEBUG")

    set_optimize("fastest")
end

target("TiltedConnect")
    set_kind("static")
    set_group("Libraries")
    add_files("Code/connect/src/*.cpp")
    add_includedirs("Code/connect/include/", {public = true})
    add_headerfiles("Code/connect/include/*.hpp", {prefixdir = "TiltedConnect"})
    add_packages("tiltedcore", "hopscotch-map", "snappy", "gamenetworkingsockets", "libuv")
    add_cxflags("-fPIC")
    add_defines("STEAMNETWORKINGSOCKETS_STATIC_LINK")
