import { ReactNode } from "react";

export type TInfoIndicatorProps = {
  label: ReactNode;
  value: ReactNode;
  variant: "default" | "success" | "error" | "warning";
};

const mapVariantClass: Record<TInfoIndicatorProps["variant"], string> = {
  default: "text-slate-400",
  success: "text-green-500",
  error: "text-red-500",
  warning: "text-yellow-400",
};

export function InfoIndicator({
  label,
  value,
  variant = "default",
}: TInfoIndicatorProps) {
  return (
    <div className="flex items-baseline gap-1 whitespace-nowrap flex-nowrap">
      <b className="font-semibold text-slate-200">{label}:</b>
      <span className={`font-bold ${mapVariantClass[variant]}`}>{value}</span>
    </div>
  );
}
