project('stm32h7xx_bin', 'c',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'c_std=gnu11',
  ],
  meson_version: '>=0.60.0')

# 设置交叉编译工具链配置文件
cross_file = 'tools/toolchains/arm-none-eabi-gcc.txt'

# 全局编译参数
c_args = [
  '-DUSE_HAL_DRIVER',
  '-DTINY_STDIO',
  '-DQF_NDUMMY_SYS',
  '-DSTM32H723xx',
  '-mcpu=cortex-m4',
  '-mthumb',
  '-mfpu=fpv4-sp-d16',
  '-mfloat-abi=hard',
  '-Wall',
  # '-finstrument-functions',
  # '-finstrument-functions-exclude-file-list=cmsis,qpc,cm_backtrace',
  # '-finstrument-functions-exclude-file-list=applications/main.c,applications/syscall.c',
  '-fno-omit-frame-pointer',
  '-ffunction-sections',
  '-fdata-sections',
  '-specs=picolibc.specs',
  '-O0',                    # 优化等级2
  '-fdata-sections',        # 将数据放入独立段
  '-ffunction-sections',    # 将函数放入独立段
]

# 全局链接参数
link_args = [
  '-mcpu=cortex-m4',
  '-mthumb',
  '-mfpu=fpv4-sp-d16',
  '-mfloat-abi=hard',
  '-Wl,--gc-sections',
  '-specs=picolibc.specs',
  '-Xlinker',
  '-Map=' + meson.current_build_dir() + '/stm32h7xx.map',
  '-T', meson.current_source_dir() + '/stm32h7xx.ld',
  '-Wl,--gc-sections',     # 删除未使用的段
  '-Wl,--print-memory-usage', # 打印内存使用情况
]

# 包含子目录构建脚本
subdir('firmware')
subdir('applications')
subdir('drivers')
subdir('middlewares/qpc')
subdir('middlewares/cm_backtrace')
subdir('middlewares/easylogger')
subdir('middlewares/libjpeg')
subdir('middlewares/MultiButton')
subdir('middlewares/trace')
subdir('middlewares/CherryUSB')


appdep_incdirs = []
appdep_sources = []
appdep_incdirs += libjpeg_incs
appdep_sources += libjpeg_srcs
appdep_incdirs += cherryusb_incs
appdep_sources += cherryusb_srcs
# appdep_incdirs += log_incs
# appdep_sources += log_srcs
if get_option('buildtype').startswith('debug')
  message('Use debug buildtype ' + get_option('buildtype'))
  appdep_sources += cmb_srcs
  appdep_incdirs += [cmb_incs, trace_incdirs]
  c_args += '-DCM_BACKTRACE_ENABLE'  # 添加编译宏以启用相关代码
  c_args += '-DCMB_USER_CFG'
  c_args += '-DDEBUG'
endif

# 创建最终可执行文件
stm32h7xx_elf = executable('stm32h7xx',
  sources: [ 
    applications_sources,
    appdep_sources,
    drivers_sources,
    qpc_sources,
    btn_sources,
    firmware_sources,
  ],
  include_directories: [
    applications_incdirs,
    appdep_incdirs,
    drivers_incdirs,
    qpc_incdirs,
    btn_incdirs,
    firmware_incdirs,
  ],
  # dependencies: [
  #   firmware_dep,  # 使用 firmware 模块的依赖声明
  # ],
  c_args: c_args,
  link_args: link_args,
  name_suffix: 'elf'
)

# 配置objcopy工具
objcopy = find_program('arm-none-eabi-objcopy')
objdump = find_program('arm-none-eabi-objdump')
# 根据操作系统选择JLink可执行文件名称

jlink_exe = 'JLinkExe'
if build_machine.system() == 'windows'
  jlink_exe = 'JLink'
endif

# 配置JLink工具
jlink = find_program(jlink_exe)

# 添加hex文件生成目标
custom_target('hex',
  input: stm32h7xx_elf,
  output: 'stm32h7xx.hex',
  command: [objcopy, '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
  build_by_default: true
)

run_target('disasm',
  command: [
    'sh', '-c',
    objdump.full_path() + ' -d -S -l -C ' + stm32h7xx_elf.full_path() + ' > ' + 
    meson.current_build_dir() / 'stm32h7xx.dis'
  ]
)

# 添加下载目标
if get_option('JLINK_REMOTE') != 'disabled'
  message('JLink remote debug enabled')
  run_target('flash',
    command: [
      jlink,
      '-ip', get_option('JLINK_REMOTE'),
      '-device', 'STM32H723ZG',
      '-if', 'SWD',
      '-speed', '4000',
      '-autoconnect', '1',
      '-CommanderScript', meson.current_source_dir() + '/tools/flash.jlink'
    ]
  )
else
  run_target('flash',
    command: [
      jlink,
      '-device', 'STM32H723ZG',
      '-if', 'SWD',
      '-speed', '4000',
      '-autoconnect', '1',
      '-CommanderScript', meson.current_source_dir() + '/tools/flash.jlink'
    ]
  )
endif

