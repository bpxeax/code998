GLOBAL = {
    LIB_OUT_DIR = "output/lib/%{cfg.buildcfg}/%{cfg.platform}",
    BIN_OUT_DIR = "output/bin/%{cfg.buildcfg}/%{cfg.platform}",
    PACKAGE_DIR = "package/%{cfg.buildcfg}/%{cfg.platform}",
    BUILD_DIR = "build/".._ACTION,
    ROOT_DIR = os.getcwd()
};