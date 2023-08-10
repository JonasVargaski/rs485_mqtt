import { TMqttData } from "@/hooks/useMqttDevice";
import { OutputIndicator } from "./output-indicator";
import { TemperatureIndicator } from "./temperature-indicator";
import { AlarmIndicator } from "./alarms-indicator";
import Image from "next/image";

type TScreenDeviceProps = {
  device: TMqttData;
};

export function ScreenDevice({ device: { data } }: TScreenDeviceProps) {
  return (
    <section className="w-[743px] select-none border rounded-md bg-gray-900">
      <div className="flex items-center px-3 w-full h-11 gap-4">
        <div className="pr-1">
          <Image
            src={`/wifi-${data.wifi_quality}.svg`}
            alt="wifi"
            width={23}
            height={23}
          />
        </div>
        <AlarmIndicator alarms={data.alarms} />
      </div>

      <table className="uppercase font-semibold text-center text-white w-full px-1">
        <thead>
          <tr className="text-black h-10">
            <th className="border border-gray-600 w-1/6 bg-yellow-200">
              Sensor temp.
            </th>
            <th className="border border-gray-600 w-1/4 bg-pink-500">
              Temperatura média
            </th>
            <th className="border border-gray-600 w-1/4 bg-blue-300">
              Umidade
            </th>
            <th className="border border-gray-600 w-1/4 bg-cyan-400">Dia</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <TemperatureIndicator sensor={1} value={data.main.temp1} />
            <td rowSpan={3} className="text-5xl border border-gray-600">
              {data.main.tempM} °C
            </td>
            <td rowSpan={2} className="text-4xl border border-gray-600">
              {data.main.umid} %
            </td>
            <td
              rowSpan={2}
              className="text-4xl border border-gray-600 normal-case"
            >
              {data.main.day}
            </td>
          </tr>
          <tr>
            <TemperatureIndicator sensor={2} value={data.main.temp2} />
          </tr>
          <tr>
            <TemperatureIndicator sensor={3} value={data.main.temp3} />
            <td className="w-1/4 bg-green-300 text-black font-bold border border-gray-600">
              Pressão
            </td>
            <td className="w-1/4 bg-orange-200 text-black font-bold border border-gray-600">
              Co2
            </td>
          </tr>
          <tr>
            <TemperatureIndicator sensor={4} value={data.main.temp4} />
            <td className="w-1/4 bg-gray-300 text-black font-bold border border-gray-600">
              Temp. Ajustada
            </td>
            <td
              rowSpan={2}
              className="text-4xl border border-gray-600 normal-case"
            >
              {data.main.pressure} Pa
            </td>
            <td
              rowSpan={2}
              className="text-4xl border border-gray-600 normal-case"
            >
              {data.main.co2} ppm
            </td>
          </tr>
          <tr>
            <TemperatureIndicator sensor={5} value={data.main.temp5} />
            <td className="text-lg font-semibold border border-gray-600">
              {data.main.tempA} °C
            </td>
          </tr>
        </tbody>
      </table>

      <div className="grid grid-cols-4 gap-2 py-2 px-3 border-t-2 border-gray-600">
        <div className="space-y-1">
          <OutputIndicator
            label="Ventilação minima"
            enabled={data.outputs.minimumVentilation}
          />
          <OutputIndicator label="Exaustor 1" enabled={data.outputs.exhaust1} />
          <OutputIndicator label="Exaustor 2" enabled={data.outputs.exhaust2} />
          <OutputIndicator label="Exaustor 3" enabled={data.outputs.exhaust3} />
        </div>
        <div className="space-y-1">
          <OutputIndicator label="Exaustor 4" enabled={data.outputs.exhaust4} />
          <OutputIndicator label="Exaustor 5" enabled={data.outputs.exhaust5} />
          <OutputIndicator label="Exaustor 6" enabled={data.outputs.exhaust6} />
          <OutputIndicator label="Exaustor 7" enabled={data.outputs.exhaust7} />
        </div>
        <div className="space-y-1">
          <OutputIndicator label="Exaustor 8" enabled={data.outputs.exhaust8} />
          <OutputIndicator label="Exaustor 9" enabled={data.outputs.exhaust9} />
          <OutputIndicator
            label="Exaustor 10"
            enabled={data.outputs.exhaust10}
          />
          <OutputIndicator
            label="Exaustor 11"
            enabled={data.outputs.exhaust11}
          />
        </div>
        <div className="space-y-1">
          <OutputIndicator
            label="Exaustor 12"
            enabled={data.outputs.exhaust12}
          />
          <OutputIndicator
            label="Exaustor 13"
            enabled={data.outputs.exhaust13}
          />
          <OutputIndicator
            label="Exaustor 14"
            enabled={data.outputs.exhaust14}
          />
          <OutputIndicator
            label="Exaustor 15"
            enabled={data.outputs.exhaust15}
          />
        </div>
      </div>
      <div className="grid grid-cols-4 gap-2 w-full py-2 px-3 border-t border-gray-600 ">
        <div className="space-y-1">
          <OutputIndicator
            label="Cortina abre"
            enabled={data.outputs.curtainOpen}
          />
          <OutputIndicator
            label="Cortina fecha"
            enabled={data.outputs.curtainClose}
          />
          <OutputIndicator
            label="Inlet abre"
            enabled={data.outputs.inletOpen}
          />
          <OutputIndicator
            label="Inlet fecha"
            enabled={data.outputs.inletClose}
          />
        </div>
        <div className="space-y-1">
          <OutputIndicator label="Aquecedor 1" enabled={data.outputs.heater1} />
          <OutputIndicator label="Aquecedor 2" enabled={data.outputs.heater2} />
          <OutputIndicator label="Aquecedor 3" enabled={data.outputs.heater3} />
          <OutputIndicator label="Aquecedor 4" enabled={data.outputs.heater4} />
        </div>
        <div className="space-y-1 col-span-2">
          <OutputIndicator
            label="Placa evaporativa"
            enabled={data.outputs.evaporativePlate}
          />
          <OutputIndicator
            label="Nebulizador"
            enabled={data.outputs.nebulizer}
          />
          <OutputIndicator
            label="Circulador pinteira"
            enabled={data.outputs.circulator}
          />
          <OutputIndicator label="Iluminação" enabled={data.outputs.lighting} />
        </div>
      </div>
    </section>
  );
}
