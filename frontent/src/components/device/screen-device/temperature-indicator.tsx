type TTemperatureIndicatorProps = {
  sensor: number;
  value?: number;
};

export function TemperatureIndicator({
  sensor,
  value = 0,
}: TTemperatureIndicatorProps) {
  return (
    <td className="border border-gray-600">
      <div className="flex items-center w-full">
        <div className="text-base font-semibold w-10 flex items-center justify-center py-1 border-r border-gray-600">
          T{sensor}
        </div>
        <div className="text-lg font-semibold flex-1 text-center w-full">
          {value} °C
        </div>
      </div>
    </td>
  );
}
