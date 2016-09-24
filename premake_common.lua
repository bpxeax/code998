GLOBAL = {}
GLOBAL.ROOT_DIR = os.getcwd()

GLOBAL.OUT_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "output")
GLOBAL.BUILD_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "build")
GLOBAL.PACKAGE_ROOT_DIR = path.join(GLOBAL.ROOT_DIR, "package")

GLOBAL.LIB_OUT_DIR = path.join(GLOBAL.OUT_ROOT_DIR, "lib/%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.BIN_OUT_DIR = path.join(GLOBAL.OUT_ROOT_DIR, "bin/%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.PACKAGE_DIR = path.join(GLOBAL.PACKAGE_ROOT_DIR, "%{cfg.buildcfg}/%{cfg.platform}")
GLOBAL.SRC_DIR = path.join(GLOBAL.ROOT_DIR, "src")
GLOBAL.BUILD_DIR = path.join(GLOBAL.BUILD_ROOT_DIR, _ACTION)