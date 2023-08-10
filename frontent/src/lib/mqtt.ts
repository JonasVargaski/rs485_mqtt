import { connect, IClientOptions } from "mqtt";

const base_topic: string = "c5d81ff2/device/";
const host: string = "wss://mqtt-dashboard.com:8884/mqtt";
const options: IClientOptions = {
  clientId: "mqtt_client_" + Math.random().toString(16).substring(2, 8),
  protocolId: "MQTT",
  protocolVersion: 4,
  clean: true,
  reconnectPeriod: 5000,
  connectTimeout: 15 * 1000,
};

const mqttClient = connect(host, options);

export { base_topic, mqttClient };
