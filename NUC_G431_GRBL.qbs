import qbs
import qbs.File
import qbs.FileInfo
import qbs.ModUtils
import qbs.TextFile

//import "size.js" as Size
Project {
    id: project

    name: {
        return /.+\/(\S+)$/.exec(project.sourceDirectory)[1]
    }

    readonly property string Makefile: TextFile(FileInfo.joinPaths(path, 'Makefile')).readAll()

    readonly property string CPU: /CPU = (\S+)/gm.exec(Makefile)[1]
    readonly property string FPU: /FPU = (\S+)/gm.exec(Makefile)[1]
    readonly property string FLOAT_ABI: /FLOAT-ABI = (\S+)/gm.exec(Makefile)[1]
    readonly property stringList MCU: [CPU, '-mthumb', FPU, FLOAT_ABI]

    readonly property string LDSCRIPT: /LDSCRIPT = (\S+.ld)/gm.exec(Makefile)[1]
    readonly property string LD: TextFile(FileInfo.joinPaths(path, LDSCRIPT)).readAll()

    readonly property stringList DEFINES: {
        var defines = [// 'USE_FULL_ASSERT',
                       'configUSE_TRACE_FACILITY=1', 'configUSE_STATS_FORMATTING_FUNCTIONS=1']
        Makefile.replace(/^-D(\S+)/gm, function (wholeMatch, def) {
            defines.push(def)
            return wholeMatch
        })
        return defines
    }

    readonly property stringList INCLUDE_PATHS: {
        var includePaths = []
        Makefile.replace(/^-I(\S+)/gm, function (wholeMatch, def) {
            includePaths.push(def)
            return wholeMatch
        })
        return includePaths
    }

    readonly property stringList COMMON_COMPILER_FLAGS: {
        var args = ['-c'].concat(project.MCU)
        for (var i in project.DEFINES)
            args.push('-D' + project.DEFINES[i])
        const out = project.sourceDirectory + '/'
        for (var i in project.INCLUDE_PATHS)
            args.push('-I' + out + project.INCLUDE_PATHS[i])
        args.push('-O0', '-Wall', '-fdata-sections', '-ffunction-sections', '-g', '-gdwarf-2', '-MMD', '-MP')
        return args
    }

    CppApplication {
        id: cppApplication
        name: project.name
        condition: {
            return (qbs.toolchain.contains('gcc') && !qbs.toolchain.contains('xcode'))
        }

        //type: [/*'elf', */ 'application'] //, 'bin_hex_size'] //, 'application', 'bin_hex_size']
        type: ['elf']

        consoleApplication: true
        cpp.cLanguageVersion: 'c17'
        cpp.cxxLanguageVersion: 'c++20'
        cpp.debugInformation: true //qbs.buildVariant !== 'release'
        cpp.enableExceptions: false
        cpp.enableRtti: false
        cpp.executableSuffix: '.elf'
        cpp.optimization: qbs.buildVariant === 'release' ? 'fast' : 'none'
        cpp.positionIndependentCode: false

        cpp.includePaths: INCLUDE_PATHS
        cpp.defines: DEFINES
        cpp.driverFlags: MCU
        // cpp.commonCompilerFlags: ['-Og', '-Wall', '-fdata-sections', '-ffunction-sections', '-g', '-gdwarf-2']
        // cpp.cFlags: ['-Og', '-Wall', '-fdata-sections', '-ffunction-sections', '-g', '-gdwarf-2'] //, '-MMD', '-MP']
        // cpp.assemblerMode: 'manual'
        // cpp.assemblerName: 'gcc'
        // cpp.assemblerFlags: ['-x', 'assembler-with-cpp'].concat(MCU)
        // cpp.driverFlags: MCU.concat('-Wall', '-Wno-unused-parameter', '-fdata-sections', '-ffunction-sections', '-finline-functions',
        // cpp.cFlags: MCU.concat('-Wall', '-fdata-sections', '-ffunction-sections')
        cpp.cxxFlags: ['-Wno-volatile', '-fno-exceptions', '-fno-rtti']
        cpp.linkerMode: 'manual'
        cpp.linkerName: 'gcc'
        cpp.driverLinkerFlags: ['-specs=nano.specs', '-Wl,--start-group', '-lc', '-lm', '-lnosys', '-lstdc++', '-Wl,--end-group', /*'-Wl,-Map=TASKS.map,--cref',*/ '-Wl,--gc-sections']

        Group {
            name: 'cpp'
            prefix: path
            fileTags: ['cpp']
            files: ['/**/*.cpp']
            excludeFiles: ['/**/DSP/**/', '/**/RTOS2/**/', '/**/RTOS/**/', '/**/NN/**/', '/**/Template/**/', '/**/Core_A/**/' //                '/**/main.c',
            ]
        }

        Group {
            name: 'c'
            prefix: path
            fileTags: ['c']
            files: ['/**/*.c']
            // excludeFiles: ['/**/main.c']
            excludeFiles: ['/**/DSP/**', '/**/NN/**', '/**/RTOS2/**', '/**/Template*/**', '/**/**template*']
        }

        Group {
            name: 'linkerscript'
            fileTags: 'linkerscript'
            prefix: path
            files: ['/**/*.ld']
            excludeFiles: ['/**/*M.ld']
        }

        Group {
            name: 'asm'
            fileTags: 'asm'
            prefix: path
            files: ['/**/*.s']
            excludeFiles: ['/**/Templates/**', '/**/NN/**', '/**/DSP/**', '/**/MDK-ARM/**']
        }

        Group {
            name: 'hpp'
            fileTags: 'hpp'
            prefix: path
            files: ['/**/*.h', '/**/*.hpp']
            excludeFiles: ['/**/Templates/**', '/**/NN/**', '/**/DSP/**']
        }

        Rule {
            id: asm_compiler
            inputs: ['asm']
            //alwaysRun: true
            Artifact {
                fileTags: ["_obj_"]
                filePath: input.fileName + ".o" //product.buildDirectory + '/' + FileInfo.baseName(input.fileName) + ".o"
            }
            prepare: {
                const out = product.buildDirectory + '/' + input.fileName // project.sourceDirectory + '/build/' + FileInfo.baseName(input.fileName)
                var args = ['-x', 'assembler-with-cpp'].concat(project.COMMON_COMPILER_FLAGS)
                args.push('-MF' + out + '.d')
                args.push(input.filePath, '-o', output.filePath)
                var cmdCompiler = new Command('arm-none-eabi-gcc', args) //Size.args('asm', input, output))
                cmdCompiler.description = "compiling " + input.fileName
                cmdCompiler.highlight = "compiler"
                return cmdCompiler
            }
        }

        Rule {
            id: c_compiler
            inputs: ['c']
            //alwaysRun: true
            Artifact {
                fileTags: ["_obj_"]
                filePath: input.fileName + ".o" //product.buildDirectory + '/' + FileInfo.baseName(input.fileName) + ".o"
            }
            prepare: {
                var args_ = input.cpp.defines.filter(function (item, i, ar) {
                    return ar.indexOf(item) === i
                })

                const out = product.buildDirectory + '/' + input.fileName //  // project.sourceDirectory + '/build/' + FileInfo.baseName(input.fileName)
                var args = [].concat(project.COMMON_COMPILER_FLAGS)
                args.push('-MF' + out + '.d', '-std=c99', '-Wa,-a,-ad,-alms=' + out + '.lst') // c
                args.push(input.filePath, '-o', output.filePath)
                var cmdCompiler = new Command('arm-none-eabi-gcc', args) // Size.args('c', input, output))
                cmdCompiler.description = "compiling " + input.fileName
                cmdCompiler.highlight = "compiler"
                return cmdCompiler
            }
        }

        Rule {
            id: cpp_compiler
            inputs: ['cpp']
            //alwaysRun: true
            Artifact {
                fileTags: ["_obj_"]
                filePath: input.fileName + ".o" //product.buildDirectory + '/' + FileInfo.baseName(input.fileName) + ".o"
            }
            prepare: {
                const out = product.buildDirectory + '/' + input.fileName //project.sourceDirectory + '/build/' + FileInfo.baseName(input.fileName)
                var args = [].concat(project.COMMON_COMPILER_FLAGS)
                args.push('-MF' + out + '.d', '-std=c++20', '-Wno-volatile', '-Wno-unused-variable', '-fno-rtti', '-fno-exceptions') //cpp
                args.push(input.filePath, '-o', output.filePath)
                var compilerPath = 'arm-none-eabi-gcc'
                var cmdCompiler = new Command(compilerPath, args)
                cmdCompiler.description = "compiling " + input.fileName
                cmdCompiler.highlight = "compiler"
                return cmdCompiler
            }
        }

        Rule {
            id: linker
            multiplex: true
            inputs: ["_obj_"]
            alwaysRun: true
            Artifact {
                fileTags: ["elf"]
                filePath: project.name + ".elf"
            }
            prepare: {
                //throw output.filePath
                var args = project.MCU

                for (i in inputs["_obj_"])
                    args.push(inputs["_obj_"][i].filePath)

                args.push('-specs=nano.specs', '-T' + project.sourceDirectory + '/' + project.LDSCRIPT, '-Wl,--start-group', '-lc', '-lm',
                          '-lnosys', '-lstdc++', '-Wl,--end-group', '-Wl,-Map=' + project.sourceDirectory + '/build/' + project.name + '.map,--cref',
                          '-Wl,--gc-sections', '-o', output.filePath)

                var linkerPath = 'arm-none-eabi-gcc'

                var cmdLinker = new Command(linkerPath, args)
                cmdLinker.description = "linking " + project.name
                cmdLinker.highlight = "linker"

                var fileName = FileInfo.fileName(output.filePath)
                var outputFileName = project.path + '/' + project.name

                var objCopyPath = 'arm-none-eabi-objcopy'
                var cmdBin = new Command(objCopyPath, ['-O', 'binary', output.filePath, outputFileName + '.bin'])
                cmdBin.description = 'converting to BIN: ' + fileName + ' -> ' + output.baseName + '.bin'
                var cmdHex = new Command(objCopyPath, ['-O', 'ihex', output.filePath, outputFileName + '.hex'])
                cmdHex.description = 'converting to HEX: ' + fileName + ' -> ' + output.baseName + '.hex'

                var objSize = 'arm-none-eabi-size'
                var argsSize = [output.filePath]
                var cmdSize = new Command(objSize, argsSize)
                cmdSize.description = 'Size: ' + fileName
                cmdSize.highlight = 'compiler'
                cmdSize.stdoutFilterFunction = function size(stdout) {
                    const sizeOfRam = 128 * 1024
                    const sizeOfFlash = 1024 * 1024
                    const regex = /(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+.+/g
                    const result = regex.exec(stdout)
                    stdout += '  13456     124   18164   31744    7c00 -> build/TASKS.elf'
                    var size = '===Used Ram  : ' + (((+result[2] + +result[3]) / sizeOfRam) * 100).toFixed(3) + ' %'
                    size += ' Used Flash: ' + ((result[1] / sizeOfFlash) * 100).toFixed(3) + ' %==='

                    // /home/x-ray/ARM_MCU/FreeRTOS_code/BOOK_CODE/01_Tasks/Core/Src/main.c: In function 'StartTask03':
                    // /home/x-ray/ARM_MCU/FreeRTOS_code/BOOK_CODE/01_Tasks/Core/Src/main.c:331:22: warning: unused variable 'xDelay3ms' [-Wunused-variable]
                    //   331 |     const TickType_t xDelay3ms = pdMS_TO_TICKS(3);
                    //       |                      ^~~~~~~~~
                    throw size
                    return stdout
                }
                return [cmdLinker, cmdBin, cmdHex, cmdSize]
            }
        }

        //        Rule {
        //            id: rule
        //            inputs: ['application']
        //            alwaysRun: true

        //            Artifact {
        //                fileTags: ['bin_hex_size']
        //                //                filePath: input.baseDir + '/' + input.baseName + '.bin'
        //            }

        //            prepare: {
        //                var objCopyPath = 'arm-none-eabi-objcopy'
        //                var argsConvBin = ['-O', 'binary', input.filePath, project.path + '/' + project.name + '.bin']
        //                var cmdBin = new Command(objCopyPath, argsConvBin)
        //                cmdBin.description = 'converting to BIN: ' + FileInfo.fileName(input.filePath) + ' -> ' + input.baseName + '.bin'

        //                var argsConvHex = ['-O', 'ihex', input.filePath, project.path + '/' /*'E:/'*/
        //                                   + project.name + '.hex']
        //                var cmdHex = new Command(objCopyPath, argsConvHex)
        //                cmdHex.description = 'converting to HEX: ' + FileInfo.fileName(input.filePath) + ' -> ' + input.baseName + '.hex'

        //                var objSize = 'arm-none-eabi-size'
        //                var argsSize = [input.filePath]
        //                var cmdSize = new Command(objSize, argsSize)
        //                cmdSize.highlight = 'linker'
        //                //                        "compiler" indicates that the command processes source code
        //                //                        "linker"   indicates that the command links objects
        //                //                        "codegen"  indicates that the command generates source code
        //                //                        "filegen"  indicates that the command creates arbitrary files
        //                cmdSize.description = 'Size: ' + FileInfo.fileName(input.filePath)
        //                cmdSize.stdoutFilterFunction = function size(stdout) {
        //                    const sizeOfRam = 128 * 1024
        //                    const sizeOfFlash = 1024 * 1024
        //                    const regex = /(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+.+/g
        //                    const result = regex.exec(stdout)

        //                    //                    stdout = "ld: libc.a(libc_a-closer.o): in function `_close_r':"
        //                    //                            + stdout // closer.c:(.text._close_r+0xc): warning: _close is not implemented and will always fail
        //                    stdout += '\n  13456     124   18164   31744    7c00 -> build/TASKS.elf'
        //                    stdout += '\nSize Of Ram  : ' + (((result[2] * 1 + result[3] * 1) / sizeOfRam) * 100).toFixed(2) + ' %'
        //                    stdout += '\nSize Of Flash: ' + ((result[1] / sizeOfFlash) * 100).toFixed(2) + ' %'
        //                    //throw stdout
        //                    return stdout
        //                }

        //                //                //            // Запись в nor память по qspi
        //                //                //            // Write to the nor memory by qspi
        //                //                //            var argsFlashingQspi =
        //                //                //                    [           '-f', 'board/stm32f746g-disco.cfg',
        //                //                //                     '-c', 'init',
        //                //                //                     '-c', 'reset init',
        //                //                //                     '-c', 'flash write_bank 1 ' + output.filePath + ' 0',
        //                //                //                     '-c', 'reset',
        //                //                //                     '-c', 'shutdown'
        //                //                //                    ]

        //                //                //            var cmdFlashQspi = new Command('openocd', argsFlashingQspi);
        //                //                //            cmdFlashQspi.description = 'Wrtie to the NOR QSPI'

        //                //                //            // Запись во внутреннюю память
        //                //                //            // Write to the internal memory
        //                //                //            var argsFlashingInternalFlash =
        //                //                //                    [           '-f', 'board/stm32f746g-disco.cfg',
        //                //                //                     '-c', 'init',
        //                //                //                     '-c', 'reset init',
        //                //                //                     '-c', 'flash write_image erase ' + input.filePath,
        //                //                //                     '-c', 'reset',
        //                //                //                     '-c', 'shutdown'
        //                //                //                    ]

        //                //                //            var cmdFlashInternalFlash = new Command('openocd', argsFlashingInternalFlash);
        //                //                //            cmdFlashInternalFlash.description = 'Wrtie to the internal flash'
        //                return [cmdBin, cmdHex, cmdSize] //cmdFlashQspi, cmdFlashInternalFlash
        //            }
        //        }
    }
}
