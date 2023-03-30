import math
import sys

list_benchmarks = [
    '500.perlbench_r',
    '502.gcc_r',
    '503.bwaves_r',
    '505.mcf_r',
    '507.cactuBSSN_r',
    '508.namd_r',
    '510.parest_r',
    '511.povray_r',
    '519.lbm_r',
    '520.omnetpp_r',
    '521.wrf_r',
    '523.xalancbmk_r',
    '525.x264_r',
    '526.blender_r',
    '527.cam4_r',
    '531.deepsjeng_r',
    '538.imagick_r',
    '541.leela_r',
    '544.nab_r',
    '548.exchange2_r',
    '549.fotonik3d_r',
    '554.roms_r',
    '557.xz_r',
    '600.perlbench_s',
    '602.gcc_s',
    '603.bwaves_s',
    '605.mcf_s',
    '607.cactuBSSN_s',
    '619.lbm_s',
    '620.omnetpp_s',
    '621.wrf_s',
    '623.xalancbmk_s',
    '625.x264_s',
    '627.cam4_s',
    '628.pop2_s',
    '631.deepsjeng_s',
    '638.imagick_s',
    '641.leela_s',
    '644.nab_s',
    '648.exchange2_s',
    '649.fotonik3d_s',
    '654.roms_s',
    '657.xz_s',
]

size = [
    '32',
    '64',
    '128',
    '256',
    '512',
    '1024',
]


def print_cronjobs(mnt, hour):
    for benchmark in list_benchmarks:
        bench = benchmark.split(".")[1]
        for s in size:
            print(
                f"{mnt} {hour} * * * ~/sim/gem5-countmin/bechmarks/{bench}/{bench}_{s}.sh > ~/stats/{bench}/cron_out/{bench}_{s}.txt")
        mnt = mnt + 10
        if mnt >= 60:
            hour = hour + math.floor(mnt / 60)
            mnt = mnt % 60
        if hour >= 23:
            hour = hour % 23


if __name__ == '__main__':
    print("Cronjobs for benchmarks.")
    if len(sys.argv) <= 2:
        print("Give proper Time for Cronjob!!!")
        exit(1)
    h = int(sys.argv[1])
    m = int(sys.argv[2])
    print_cronjobs(m, h)
