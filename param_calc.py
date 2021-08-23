from math import *

# script that calculates idle days, TX presses and OCR1 settings
# all data are ideal, from corresponding datasheets

battery_capacity_mAh = 200
rf24_pd_current_mA = 900e-6
rf24_rx_current_mA = {.25: 12.6, 1: 13.1, 2: 13.5}
rf24_tx_current_mA = {0: 11.3, -6: 9.0, -12: 7.5, -18: 7.0}
m48_wdt_current_mA = 3.75e-3
m48_1MHz_active_current_mA = .6
m48_ps_current_mA = 7e-3


def get_idle_days(rx_time_s, rf24_air_speed_Mbps=2):
    return battery_capacity_mAh / ((1 - rx_time_s) * (m48_wdt_current_mA + m48_ps_current_mA + rf24_pd_current_mA)
                                   + rx_time_s * (m48_wdt_current_mA + m48_1MHz_active_current_mA + rf24_rx_current_mA[rf24_air_speed_Mbps])) / 24


def get_tx_info(tx_time_s, rf24_tx_power_dBm=-6):
    prescaler = 1024
    return {
        'min_press': battery_capacity_mAh * 3600 / (tx_time_s * (m48_wdt_current_mA + m48_1MHz_active_current_mA + rf24_tx_current_mA[rf24_tx_power_dBm])),
        'timer_1_comp_val': hex(ceil(tx_time_s / (prescaler / 1e6)))
    }


rx_time_s = 2e-3
tx_time_s = 2.003

print(f'idle {get_idle_days(rx_time_s)} days when rx {rx_time_s * 1e3}ms')
print(f'tx {tx_time_s}s:', get_tx_info(tx_time_s))
