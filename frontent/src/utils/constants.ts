import { TInfoIndicatorProps } from "@/components/info-indicator";
import { TConnectionStatus } from "@/hooks/useMqttDevice";

enum eHolding {
  "({Link2}1@W40001)", // ({Link1}1@D200) -> temp 1",
  "({Link2}1@W40002)", // ({Link1}1@D202) -> temp 2",
  "({Link2}1@W40003)", // ({Link1}1@D204) -> temp 3",
  "({Link2}1@W40004)", // ({Link1}1@D206) -> temp 4",
  "({Link2}1@W40005)", // ({Link1}1@D208) -> temp 5",
  "({Link2}1@W40006)", // ({Link1}1@D210) -> temp media",
  "({Link2}1@W40007)", // ({Link1}1@D598) -> temp ajustada",
  "({Link2}1@W40008)", // ({Link1}1@D214) -> umidade",
  "({Link2}1@W40009)", // ({Link1}1@D216) -> pressao",
  "({Link2}1@W40010)", // ({Link1}1@D408) -> dia",
  "({Link2}1@W40011)", // ({Link1}1@D218) -> co2",
}

enum eCoil {
  "({Link2}1@B1)", // ({Link1}1@M80) -> ventilação minima
  "({Link2}1@B2)", // ({Link1}1@Y27) -> exaustor 1
  "({Link2}1@B3)", // ({Link1}1@Y30) -> exaustor 2
  "({Link2}1@B4)", // ({Link1}1@Y31) -> exaustor 3
  "({Link2}1@B5)", // ({Link1}1@Y32) -> exaustor 4
  "({Link2}1@B6)", // ({Link1}1@Y33) -> exaustor 5
  "({Link2}1@B7)", // ({Link1}1@Y34) -> exaustor 6
  "({Link2}1@B8)", // ({Link1}1@Y35) -> exaustor 7
  "({Link2}1@B9)", // ({Link1}1@Y36) -> exaustor 8
  "({Link2}1@B10)", // ({Link1}1@Y37) -> exaustor 9
  "({Link2}1@B11)", // ({Link1}1@Y40) -> exaustor 10
  "({Link2}1@B12)", // ({Link1}1@Y41) -> exaustor 11
  "({Link2}1@B13)", // ({Link1}1@Y42) -> exaustor 12
  "({Link2}1@B14)", // ({Link1}1@Y43) -> exaustor 13
  "({Link2}1@B15)", // ({Link1}1@Y44) -> exaustor 14
  "({Link2}1@B16)", // ({Link1}1@Y45) -> exaustor 15
  "({Link2}1@B17)", // ({Link1}1@Y4) -> cortina abre
  "({Link2}1@B18)", // ({Link1}1@Y5) -> cortina fecha
  "({Link2}1@B19)", // ({Link1}1@Y2) -> inlet abre
  "({Link2}1@B20)", // ({Link1}1@Y3) -> inlet fecha
  "({Link2}1@B21)", // ({Link1}1@Y23) -> aquecedor 1
  "({Link2}1@B22)", // ({Link1}1@Y24) -> aquecedor 2
  "({Link2}1@B23)", // ({Link1}1@Y25) -> aquecedor 3
  "({Link2}1@B24)", // ({Link1}1@Y26) -> aquecedor 4
  "({Link2}1@B25)", // ({Link1}1@Y0) -> placa evaporativa
  "({Link2}1@B26)", // ({Link1}1@Y1) -> nebulizador
  "({Link2}1@B27)", // ({Link1}1@Y20) -> circulador pinteira
  "({Link2}1@B28)", // ({Link1}1@Y21) -> iluminação

  "({Link2}1@B29)", // ({Link1}1@M400) -> ALARME EMERGENCIA
  "({Link2}1@B30)", // ({Link1}1@M401) -> ALARME DE TEMPERATURA MAXIMA
  "({Link2}1@B31)", // ({Link1}1@M402) -> ALARME DE TEMPERTUR MINIMA
  "({Link2}1@B32)", // ({Link1}1@M403) -> ALARME DE PRESSAO MAXIMA
  "({Link2}1@B33)", // ({Link1}1@M404) -> ALARME DE PRESSAO MINIMA
  "({Link2}1@B34)", // ({Link1}1@M405) -> ALARME AQUECEDOR
  "({Link2}1@B35)", // ({Link1}1@M406) -> ALARME UMIDADE MAXIMA
  "({Link2}1@B36)", // ({Link1}1@M407) -> ALARME UMIDADE MINIMA
  "({Link2}1@B37)", // ({Link1}1@M408) -> ALARME CO2 MAXIMA
  "({Link2}1@B38)", // ({Link1}1@M409) -> ALARME PALHETA DE SEGURANÇA
  "({Link2}1@B39)", // ({Link1}1@M410) -> ALARME TERMOSTATO DE SEGURANÇA
  "({Link2}1@B40)", // ({Link1}1@M411) -> ALARME DR1
  "({Link2}1@B41)", // ({Link1}1@M412) -> ALARME DR2
  "({Link2}1@B42)", // ({Link1}1@M413) -> ALARME DR3
  "({Link2}1@B43)", // ({Link1}1@M414) -> ALARME DR4
  "({Link2}1@B44)", // ({Link1}1@M415) -> ALARME DR5
  "({Link2}1@B45)", // ({Link1}1@M416) -> ALARME GRUPOS EXAUSTORES
  "({Link2}1@B46)", // ({Link1}1@M417) -> ALARME PLACA EVAPORATIVA
  "({Link2}1@B47)", // ({Link1}1@M418) -> ALARME NEBOLIZADOR
  "({Link2}1@B48)", // ({Link1}1@M419) -> ALARME INLET
  "({Link2}1@B49)", // ({Link1}1@M420) -> ALARME CORTINA
  "({Link2}1@B50)", // ({Link1}1@M421) -> ALARME CIRCULADOR DE AR
  "({Link2}1@B51)", // ({Link1}1@M422) -> ALARME COMEDOURO
  "({Link2}1@B52)", // ({Link1}1@M423) -> ALARME SILO
  "({Link2}1@B53)", // ({Link1}1@M424) -> ALARME FALTA ENERGIA
  "({Link2}1@B54)", // ({Link1}1@M429) -> ALARME SENSOR UMIDADE
  "({Link2}1@B55)", // ({Link1}1@M430) -> ALARME SENSOR PRESSAO
  "({Link2}1@B56)", // ({Link1}1@M431) -> ALARME SENSOR CO2
  "({Link2}1@B57)", // ({Link1}1@M432) -> ALARME SENSOR TEMPERATURA 1
  "({Link2}1@B58)", // ({Link1}1@M433) -> ALARME SENSOR TEMPERATURA 2
  "({Link2}1@B59)", // ({Link1}1@M434) -> ALARME SENSOR TEMPERATURA 3
  "({Link2}1@B60)", // ({Link1}1@M435) -> ALARME SENSOR TEMPERATURA 4
  "({Link2}1@B61)", // ({Link1}1@M436) -> ALARME SENSOR TEMPERATURA 5
}

const alarmCoilsMap = [
  eCoil["({Link2}1@B29)"],
  eCoil["({Link2}1@B30)"],
  eCoil["({Link2}1@B31)"],
  eCoil["({Link2}1@B32)"],
  eCoil["({Link2}1@B33)"],
  eCoil["({Link2}1@B34)"],
  eCoil["({Link2}1@B35)"],
  eCoil["({Link2}1@B36)"],
  eCoil["({Link2}1@B37)"],
  eCoil["({Link2}1@B38)"],
  eCoil["({Link2}1@B39)"],
  eCoil["({Link2}1@B40)"],
  eCoil["({Link2}1@B41)"],
  eCoil["({Link2}1@B42)"],
  eCoil["({Link2}1@B43)"],
  eCoil["({Link2}1@B44)"],
  eCoil["({Link2}1@B45)"],
  eCoil["({Link2}1@B46)"],
  eCoil["({Link2}1@B47)"],
  eCoil["({Link2}1@B48)"],
  eCoil["({Link2}1@B49)"],
  eCoil["({Link2}1@B50)"],
  eCoil["({Link2}1@B51)"],
  eCoil["({Link2}1@B52)"],
  eCoil["({Link2}1@B53)"],
  eCoil["({Link2}1@B54)"],
  eCoil["({Link2}1@B55)"],
  eCoil["({Link2}1@B56)"],
  eCoil["({Link2}1@B57)"],
  eCoil["({Link2}1@B58)"],
  eCoil["({Link2}1@B59)"],
  eCoil["({Link2}1@B60)"],
  eCoil["({Link2}1@B61)"],
];

const mapConnectionStatus: Record<
  TConnectionStatus,
  { label: string; variant: TInfoIndicatorProps["variant"] }
> = {
  connected: { label: "Conectado", variant: "success" },
  connecting: { label: "Conectando", variant: "default" },
  error: { label: "Erro de comunicação", variant: "error" },
  loose: { label: "Comunicação perdida", variant: "error" },
  timeout: { label: "Sem resposta", variant: "warning" },
};

const mapSignal: Record<
  number,
  { label: string; variant: TInfoIndicatorProps["variant"] }
> = {
  0: { label: "...", variant: "default" },
  1: { label: "Fraco", variant: "error" },
  2: { label: "Bom", variant: "warning" },
  3: { label: "Forte", variant: "success" },
};

const mapRS485Status: Record<
  number,
  { label: string; variant: TInfoIndicatorProps["variant"] }
> = {
  0: { label: "...", variant: "default" },
  1: { label: "Erro", variant: "error" },
  2: { label: "Conectado", variant: "success" },
};

export {
  eHolding,
  eCoil,
  alarmCoilsMap,
  mapConnectionStatus,
  mapSignal,
  mapRS485Status,
};
