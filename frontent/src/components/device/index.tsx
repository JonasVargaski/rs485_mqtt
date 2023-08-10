import { TMqttData } from "@/hooks/useMqttDevice";
import { ScreenDevice } from "./screen-device";
import { InfoIndicator } from "./info-indicator";
import {
  mapConnectionStatus,
  mapRS485Status,
  mapSignal,
} from "@/utils/constants";

type TDeviceProps = {
  data: TMqttData;
};

export function Device({ data }: TDeviceProps) {
  return (
    <div key={data.id} className="block p-2 bg-black w-[756px]">
      <div className="flex items-baseline gap-1 m-1">
        <h3 className="text-xl font-medium text-cyan-400 truncate">
          {data.name}
        </h3>
        <p className="text-xs font-semibold text-slate-400">#{data.id}</p>
      </div>
      <ScreenDevice device={data} />
      <div className="px-1 flex items-center justify-between mt-1">
        <InfoIndicator
          label="Status"
          value={mapConnectionStatus[data.status].label}
          variant={mapConnectionStatus[data.status].variant}
        />
        <InfoIndicator
          label="RS-485"
          value={mapRS485Status[data.data.rs485].label}
          variant={mapRS485Status[data.data.rs485].variant}
        />
        <InfoIndicator
          label="Qualidade do sinal"
          value={
            <>
              {mapSignal[data.data.wifi_quality].label}{" "}
              <small className="text-xs font-normal">
                ({data.data.rssi}dBm)
              </small>
            </>
          }
          variant={mapSignal[data.data.wifi_quality].variant}
        />
      </div>
    </div>
  );
}
