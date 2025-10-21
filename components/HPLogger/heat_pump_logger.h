#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include <string.h>
#include <vector>

namespace esphome {
namespace heat_pump_logger {

class HeatPumpLogger : public uart::UARTDevice, public Component {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_gas_inlet(sensor::Sensor *gas_inlet) { gas_inlet_ = gas_inlet; }
    void set_gas_outlet(sensor::Sensor *gas_outlet) { gas_outlet_ = gas_outlet; }
    void set_water_inlet(sensor::Sensor *water_inlet) { water_inlet_ = water_inlet; }
    void set_water_outlet(sensor::Sensor *water_outlet) { water_outlet_ = water_outlet; }
    void set_outdoor_temp(sensor::Sensor *outdoor_temp) { outdoor_temp_ = outdoor_temp; }
    void set_water_flow(sensor::Sensor *water_flow) { water_flow_ = water_flow; }
    void set_cop(sensor::Sensor *cop) { cop_ = cop; }
    void set_power_active(sensor::Sensor *power_active) { power_active_ = power_active; }
    void set_power_reactive(sensor::Sensor *power_reactive) { power_reactive_ = power_reactive; }
    void set_power_apparent(sensor::Sensor *power_apparent) { power_apparent_ = power_apparent; }
    void set_fwd_act_energy(sensor::Sensor *fwd_act_energy) { fwd_act_energy_ = fwd_act_energy; }
    void set_voltage(sensor::Sensor *voltage) { voltage_ = voltage; }
    void set_current(sensor::Sensor *current) { current_ = current; }
    void set_freq(sensor::Sensor *freq) { freq_ = freq; }
    void set_power_factor(sensor::Sensor *power_factor) { power_factor_ = power_factor; }
    void set_heating_power(sensor::Sensor *heating_power) { heating_power_ = heating_power; }

private:
    String buffer_;  // Bufor do przechowywania odczytanych bajtów
    int dataError;
    std::vector<float> split_buffer_to_floats(const String &buffer, char delimiter);  // Deklaracja funkcji

    sensor::Sensor *gas_inlet_{nullptr};
    sensor::Sensor *gas_outlet_{nullptr};
    sensor::Sensor *water_inlet_{nullptr};
    sensor::Sensor *water_outlet_{nullptr};
    sensor::Sensor *outdoor_temp_{nullptr};
    sensor::Sensor *water_flow_{nullptr};
    sensor::Sensor *cop_{nullptr};
    sensor::Sensor *power_active_{nullptr};
    sensor::Sensor *power_reactive_{nullptr};
    sensor::Sensor *power_apparent_{nullptr};
    sensor::Sensor *fwd_act_energy_{nullptr};
    sensor::Sensor *voltage_{nullptr};
    sensor::Sensor *current_{nullptr};
    sensor::Sensor *freq_{nullptr};
    sensor::Sensor *power_factor_{nullptr};
    sensor::Sensor *heating_power_{nullptr};
};

}  // namespace heat_pump_logger
}  // namespace esphome