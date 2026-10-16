export type DeviceSensorValue = {
    device_id?: string;
    is_discovered?: boolean;
    is_synced?: boolean;
    last_seen_ms?: number;
    moisture_percent?: number;
    temperature_c?: number;
    humidity_percent?: number;
    aqi?: number;
    tvoc_ppb?: number;
    eco2_ppm?: number;
};

export type HubStateValue = {
    device_count?: number;
    moisture_percent?: number;
    temperature_c?: number;
    humidity_percent?: number;
    aqi?: number;
    tvoc_ppb?: number;
    eco2_ppm?: number;
    devices?: DeviceSensorValue[];
};

export type SocketMessage = {
    name: string;
    type: string;
    value: string | HubStateValue;
};