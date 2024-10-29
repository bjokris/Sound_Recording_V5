
Import("env")

build_flags = env.ParseFlags(env['BUILD_FLAGS'])
flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
defines = {k: v for (k, v) in flags_with_value_list}
env.Replace(PROGNAME="fw_sound_record_" + defines.get('CONFIG_FW_VERSION').replace('"', ''))