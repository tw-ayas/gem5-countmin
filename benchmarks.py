import os

list_benchmarks = {
    '500.perlbench_r': ['', '-I {spec}/lib {spec}/checkspan.pl 2500 5 25 11 150 1 1 1 1'],
    '502.gcc_r': ['cpugcc_r', '{spec}/gcc-pp.c -O3 -finline-limit=0 -fif-conversion -fif-conversion2 -o '
                              '{spec}/gcc-pp.opts-O3_-finline-limit_0_-fif-conversion_-fif-conversion2.s'],
    '503.bwaves_r': ['', 'bwaves_1 < {spec}/bwaves_1.in'],
    '505.mcf_r': ['', '{spec}/inp.in'],
    '507.cactuBSSN_r': ['cactusBSSN_r', '{spec}/spec_ref.par'],
    '508.namd_r': ['', '--input {spec}/apoa1.input --output {spec}/apoa1.ref.output --iterations 65'],
    '510.parest_r': ['', '{spec}/ref.prm'],
    '511.povray_r': ['', '{spec}/SPEC-benchmark-ref.ini'],
    '519.lbm_r': ['', '3000 {spec}/reference.dat 0 0 {spec}/100_100_130_ldc.of'],
    '520.omnetpp_r': ['', '-c General -r 0'],
    '521.wrf_r': ['', '> {spec}/rsl.out.0000'],
    '523.xalancbmk_r': ['cpuxalan_r', '-v {spec}/t5.xml {spec}/xalanc.xsl'],
    '525.x264_r': ['', '--pass 1 --stats {spec}/x264_stats.log --bitrate 1000 '
                       '--frames 1000 -o {spec}/BuckBunny_New.264 {spec}/BuckBunny.yuv 1280x720'],
    '526.blender_r': ['', '{spec}/sh3_no_char.blend --render-output {spec}/sh3_no_char_ --threads 1 -b -F RAWTGA -s '
                          '849 -e 849 -a'],
    '527.cam4_r': ['', '> {spec}/cam4_r_base.initial-m64.txt'],
    '531.deepsjeng_r': ['', '{spec}/ref.txt'],
    '538.imagick_r': ['', '-limit disk 0 {spec}/refrate_input.tga -edge 41 -resample 181% -emboss 31 -colorspace YUV '
                          '-mean-shift 19x19+15% -resize 30% {spec}/refrate_output.tga'],
    '541.leela_r': ['', '{spec}/ref.sgf'],
    '544.nab_r': ['', '{spec}/1am0 1122214447 122'],
    '548.exchange2_r': ['', '6'],
    '549.fotonik3d_r': ['', ''],
    '554.roms_r': ['', '< {spec}/ocean_benchmark2.in.x'],
    '557.xz_r': ['', '{spec}/cld.tar.xz 160 19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff'
                     '3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474 59796407 61004416 6'],
    '600.perlbench_s': ['', '-I {spec}/lib {spec}/checkspam.pl 2500 5 25 11 150 1 1 1 1'],
    '602.gcc_s': ['sgcc', '{spec}/gcc-pp.c -O5 -finline-limit=1000 -fselective-scheduling '
                          '-fselective-scheduling2 -o {spec}/gcc-pp.opts-O5_-finline-limit_1000_'
                          '-fselective-scheduling_-fselective-scheduling2.s'],
    '603.bwaves_s': ['speed_bwaves', 'bwaves_1 < {spec}/bwaves_1.in'],
    '605.mcf_s': ['', '{spec}/inp.in'],
    '607.cactuBSSN_s': ['', '{spec}/spec_ref.par'],
    '619.lbm_s': ['', '2000 {spec}/reference.dat 0 0 {spec}/200_200_260_ldc.of'],
    '620.omnetpp_s': ['', '-c General -r 0'],
    '621.wrf_s': ['', ''],
    '623.xalancbmk_s': ['', '-v {spec}/t5.xml {spec}/xalanc.xsl'],
    '625.x264_s': ['', '--pass 1 --stats {spec}/x264_stats.log --bitrate 1000 '
                       '--frames 1000 -o {spec}BuckBunny_New.264 {spec}/BuckBunny.yuv 1280x720'],
    '627.cam4_s': ['', '> {spec}/cam4_s_base.initial-m64.txt'],
    '628.pop2_s': ['speed_pop2', ''],
    '631.deepsjeng_s': ['', '{spec}/ref.txt'],
    '638.imagick_s': ['', '-limit disk 0 {spec}/refspeed_input.tga -resize 817% -rotate -2.76 -shave 540x375 -alpha '
                          'remove -auto-level -contrast-stretch 1x1% -colorspace Lab -channel R -equalize +channel '
                          '-colorspace sRGB -define histogram:unique-colors=false -adaptive-blur 0x5 -despeckle '
                          '-auto-gamma -adaptive-sharpen 55 -enhance -brightness-contrast 10x10 -resize 30% '
                          '{spec}/refspeed_output.tga'],
    '641.leela_s': ['', '{spec}/ref.sgf'],
    '644.nab_s': ['', '{spec}/3j1n 20140317 220'],
    '648.exchange2_s': ['', '6'],
    '649.fotonik3d_s': ['', ''],
    '654.roms_s': ['', '< {spec}/ocean_benchmark3.in'],
    '657.xz_s': ['', '{spec}/cpu2006docs.tar.xz 6643 '
                     '055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca'
                     '86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae 1036078272 1111795472 4'],
}

benchmark_ignore = []

size = {
    '32': [2],
    '64': [2, 4],
    '128': [4, 8],
    '256': [4, 8],
    '512': [4, 8, 16],
    '1024': [4, 8, 16],
}

strategy = {
    0: 'nc',
    1: 'c',
    2: 'm',
    3: 'mc'}
group = [1, 2, 3, 4, 0]
path_spec_cpu = "sim/spec-cpu"
path_gem5 = "sim/gem5-countmin"
path_stats = "stats"
spec = f"$HOME/{path_spec_cpu}/benchspec/CPU/"
build = "build"

for benchmark, execut in list_benchmarks.items():
    bench = benchmark.split(".")[1]
    if bench in benchmark_ignore:
        continue
    fileDir = f"./benchmarks/{bench}/"
    files = os.listdir("./benchmarks")
    if bench not in files:
        os.mkdir(fileDir)
    for s, depth in size.items():
        fileName = fileDir + f"{bench}_{s}.sh"
        file = open(fileName, "w")
        file.write("#!/bin/sh\n")
        file.write(f"DIR=\"$HOME/{path_stats}/{bench}\"\n")
        file.write("if [ ! -d \"$DIR\" ]; then\n")
        file.write(f"    echo 'mkdir $HOME/{path_stats}/{bench}'\n")
        file.write(f"    mkdir $HOME/{path_stats}/{bench}\n")
        file.write(f"fi\n")
        file.write(f"DIR=\"$HOME/{path_stats}/{bench}/cron_out\"\n")
        file.write("if [ ! -d \"$DIR\" ]; then\n")
        file.write(f"    echo 'mkdir $HOME/{path_stats}/{bench}/cron_out'\n")
        file.write(f"    mkdir $HOME/{path_stats}/{bench}/cron_out\n")
        file.write(f"fi\n")
        for d in depth:
            for st_code, st in strategy.items():
                file.write(f"#{s}_{d}_{st}\n")
                for g in group:
                    end = '&&'
                    directory = f"{s}_{d}_{st}_{g}"
                    if d == depth[-1] and st == 'mc' and g == group[-1]:
                        end = ''
                    ref = 'refrate'
                    if bench[-1] == 's':
                        ref = 'refspeed'
                    cmd = execut[0]
                    if cmd == "":
                        cmd = bench
                    options = f"{spec}{benchmark}/run/run_base_{ref}_initial.m64.0000"
                    options = execut[1].replace("{spec}", options)
                    file.write(
                        f"$HOME/{path_gem5}/{build}/X86/gem5.opt  --outdir=$HOME/{path_stats}/{bench}/{directory} $HOME/"
                        f"{path_gem5}/configs/example/se.py --cmd=$HOME/{path_spec_cpu}/benchspec/"
                        f"CPU/{benchmark}/build/build_base_initial-m64.0000/{cmd} "
                        f"--options=\"{options}\" --mem-size=8GB --cpu-type=DerivO3CPU "
                        f"--num-cpu=1 --maxinsts=1000000 --caches --l2cache --l1d_size=32kB --l1i_size=32kB "
                        f"--l2_size=512kB --prob-hw-counters=1 --prob-hw-counters-size={s}B "
                        f"--prob-hw-counters-depth={d} --prob-hw-counters-conservative-update={st_code} "
                        f"--l1i-hwp-type=StridePrefetcher "
                        f"--l1d-hwp-type=StridePrefetcher --mem-type=DDR4_2400_4x16 "
                        f"--prob-hw-counters-group={g} >> $HOME/{path_stats}/{bench}/cron_out/{directory}.txt"
                        f" {end}\n")
                file.write("\n\n")
        file.close()
