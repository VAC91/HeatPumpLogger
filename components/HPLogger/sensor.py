import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY

DEPENDENCIES = ["uart"]

heat_pump_logger_ns = cg.esphome_ns.namespace("heat_pump_logger")
HeatPumpLogger = heat_pump_logger_ns.class_(
    "HeatPumpLogger", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(HeatPumpLogger),
            cv.Optional("gas_inlet"): sensor.sensor_schema(unit_of_measurement="°C", icon="mdi:thermometer", accuracy_decimals=1, state_class="measurement", device_class="temperature"),
            cv.Optional("gas_outlet"): sensor.sensor_schema(unit_of_measurement="°C", icon="mdi:thermometer", accuracy_decimals=1, state_class="measurement", device_class="temperature"),
            cv.Optional("water_inlet"): sensor.sensor_schema(unit_of_measurement="°C", icon="mdi:thermometer", accuracy_decimals=1, state_class="measurement", device_class="temperature"),
            cv.Optional("water_outlet"): sensor.sensor_schema(unit_of_measurement="°C", icon="mdi:thermometer", accuracy_decimals=1, state_class="measurement", device_class="temperature"),
            cv.Optional("outdoor_temp"): sensor.sensor_schema(unit_of_measurement="°C", icon="mdi:thermometer", accuracy_decimals=1, state_class="measurement", device_class="temperature"),
            cv.Optional("water_flow"): sensor.sensor_schema(unit_of_measurement="l/min", icon="mdi:waves-arrow-left", accuracy_decimals=1, state_class="measurement", device_class="volume_flow_rate"),
            cv.Optional("cop"): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon="mdi:heat-pump-outline", accuracy_decimals=1, state_class="measurement"),
            cv.Optional("power_active"): sensor.sensor_schema(unit_of_measurement="W", icon="mdi:flash", accuracy_decimals=1, state_class="measurement", device_class="power"),
            cv.Optional("power_reactive"): sensor.sensor_schema(unit_of_measurement="var", icon="mdi:flash", accuracy_decimals=1, state_class="measurement", device_class="reactive_power"),
            cv.Optional("power_apparent"): sensor.sensor_schema(unit_of_measurement="VA", icon="mdi:flash", accuracy_decimals=1, state_class="measurement", device_class="apparent_power"),
            cv.Optional("fwd_act_energy"): sensor.sensor_schema(unit_of_measurement="kWh", icon="mdi:flash", accuracy_decimals=2, state_class="total_increasing", device_class="energy"),
            cv.Optional("voltage"): sensor.sensor_schema(unit_of_measurement="V", icon="mdi:sine-wave", accuracy_decimals=1, state_class="measurement", device_class="voltage"),
            cv.Optional("current"): sensor.sensor_schema(unit_of_measurement="A", icon="mdi:current-ac", accuracy_decimals=1, state_class="measurement", device_class="current"),
            cv.Optional("freq"): sensor.sensor_schema(unit_of_measurement="Hz", icon="mdi:cosine-wave", accuracy_decimals=1, state_class="measurement", device_class="frequency"),
            cv.Optional("power_factor"): sensor.sensor_schema(unit_of_measurement="%", icon="mdi:angle-acute", accuracy_decimals=0, state_class="measurement", device_class="power_factor"),
            cv.Optional("heating_power"): sensor.sensor_schema(unit_of_measurement="W", icon="mdi:heating-coil", accuracy_decimals=0, state_class="measurement", device_class="power"),

        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    for key in [
        "gas_inlet", "gas_outlet", "water_inlet", "water_outlet", "outdoor_temp",
        "water_flow", "cop", "power_active", "power_reactive", "power_apparent",
        "fwd_act_energy", "voltage", "current", "freq", "power_factor", "heating_power"
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}")(sens))