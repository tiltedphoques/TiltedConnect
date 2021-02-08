set_languages("cxx20")

set_xmakever("2.5.1")

add_requires("tiltedcore", "hopscotch-map", "snappy", "gamenetworkingsockets")
add_requires("catch2")

add_rules("mode.debug","mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

if is_mode("release") then
    add_ldflags("/LTCG", "/OPT:REF")
    add_cxflags("/Ot", "/GL", "/Ob2", "/Oi", "/GS-")
    add_defines("NDEBUG")

    set_optimize("fastest")
end

target("TiltedConnect")
    set_kind("static")
    set_group("Libraries")
    add_files("Code/connect/src/*.cpp")
    add_includedirs("Code/connect/include/", {public = true})
    add_headerfiles("Code/connect/include/*.hpp", {prefixdir = "TiltedConnect"})
    add_packages("tiltedcore", "hopscotch-map", "snappy", "gamenetworkingsockets")
    add_cxflags("-fPIC")

target("TiltedConnect_Tests")
    set_kind("binary")
    set_group("Tests")
    add_files("Code/tests/src/*.cpp")
    add_deps("TiltedConnect")
    add_packages("catch2", "hopscotch-map", "tiltedcore", "gamenetworkingsockets")
