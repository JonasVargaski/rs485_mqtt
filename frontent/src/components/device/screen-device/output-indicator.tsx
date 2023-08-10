type TOutputIndicatorProps = {
  label: string;
  enabled: boolean;
};

export function OutputIndicator({ label, enabled }: TOutputIndicatorProps) {
  return (
    <div className="flex items-center gap-2 whitespace-nowrap">
      <div
        className={`p-2 rounded-full border-2 border-gray-200 transition-colors ${
          enabled ? "bg-green-500" : "bg-red-500"
        }`}
      />
      <div className="text-sm font-medium uppercase">{label}</div>
    </div>
  );
}
