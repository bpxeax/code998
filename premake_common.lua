GLOBAL = {}
GLOBAL.ROOT_DIR = os.getcwd()

GLOBAL.OUT_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "output")
GLOBAL.BUILD_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "build")
GLOBAL.PACKAGE_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "package")
GLOBAL.THIRD_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "3rd");

GLOBAL.LIB_OUT_DIR = path.join(GLOBAL.OUT_ROOT_DIR, "lib/%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.BIN_OUT_DIR = path.join(GLOBAL.OUT_ROOT_DIR, "bin/%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.PACKAGE_DIR = path.join(GLOBAL.PACKAGE_ROOT_DIR, "%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.BUILD_DIR = path.join(GLOBAL.BUILD_ROOT_DIR, _ACTION)

GLOBAL.THIRD_SRC_DIR = path.join(GLOBAL.THIRD_ROOT_DIR, "src");
GLOBAL.SRC_DIR = path.join(GLOBAL.ROOT_DIR, "src")
GLOBAL.GEN_SRC_DIR = path.join(GLOBAL.ROOT_DIR, "gen_src")

GLOBAL.TOOLS_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "tools")
GLOBAL.GENERATORS_DIR = path.join(GLOBAL.TOOLS_ROOT_DIR, "generators")