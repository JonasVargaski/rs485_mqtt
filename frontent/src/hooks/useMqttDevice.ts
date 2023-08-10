"use client";

import { base_topic, mqttClient } from "@/lib/mqtt";
import {
  TDeviceData,
  getDefaultDeviceData,
  parseMessageToDeviceData,
} from "@/utils/parser";
import { useEffect, useState } from "react";

export type TDevice = {
  id: string;
  name: string;
};

type TUseMqttDeviceProps = {
  devices: TDevice[];
  diffInSeconds: number;
  intervalCheckInSeconds: number;
};

export type TConnectionStatus =
  | "connecting"
  | "connected"
  | "loose"
  | "timeout"
  | "error";

export type TMqttData = TDevice & {
  data: TDeviceData;
  lastUpdate: Date;
  status: TConnectionStatus;
};

export function useMqttDevice({
  devices,
  diffInSeconds,
  intervalCheckInSeconds,
}: TUseMqttDeviceProps) {
  const [data, setData] = useState<Record<string, TMqttData>>(() =>
    devices.reduce((acc, cur) => {
      acc[cur.id] = {
        ...cur,
        lastUpdate: new Date(),
        status: "connecting",
        data: getDefaultDeviceData(),
      };
      return acc;
    }, {} as Record<string, TMqttData>)
  );

  useEffect(() => {
    mqttClient.on("message", (event, message) => {
      if (event.includes(base_topic)) {
        try {
          const deviceData = parseMessageToDeviceData(message.toString());
          setData((old) => {
            old[deviceData.id].status = "connected";
            old[deviceData.id].data = deviceData;
            old[deviceData.id].lastUpdate = new Date();
            return { ...old };
          });
        } catch (error) {
          console.log("parsing error:", { error, message });
          const [id] = event.match(/[^/]+$/) ?? [];
          if (id) {
            setData((old) => {
              old[id].data = getDefaultDeviceData();
              old[id].status = "error";
              return old;
            });
          }
        }
      }
    });
  }, []);

  useEffect(() => {
    devices.forEach((device) => {
      mqttClient.subscribe(`${base_topic}${device.id}`);
    });

    const timer = setInterval(
      () => setData((old) => ({ ...old })),
      intervalCheckInSeconds * 1000
    );
    return () => {
      clearInterval(timer);
      devices.forEach((device) => {
        mqttClient.unsubscribe(`${base_topic}${device.id}`);
      });
    };
  }, [devices, intervalCheckInSeconds]);

  const parsedDevices = Object.values(data).map<TMqttData>((device) => {
    if (
      new Date().getTime() - device.lastUpdate.getTime() >=
      diffInSeconds * 1000
    ) {
      const isTimeout = device.data.rssi !== 0;
      return {
        ...device,
        status: isTimeout ? "loose" : "timeout",
        data: getDefaultDeviceData(device.id),
      };
    }
    return device;
  });

  return parsedDevices;
}
