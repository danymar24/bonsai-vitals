import React, { useEffect, useRef, useState } from 'react';
import './App.css';
import { HubStateValue, SocketMessage } from './types';

const isDev = process.env.NODE_ENV === 'development';

function App() {
  const [hubState, setHubState] = useState<HubStateValue>({
    device_count: 0,
    devices: []
  });
  const ws = useRef<WebSocket | null>(null);

  useEffect(() => {
    ws.current = new WebSocket(`ws://${window.location.host}/ws`);

    ws.current.onopen = () => {
      console.log('Connected to WebSocket server');
    };

    ws.current.onmessage = (event: MessageEvent) => {
      try {
        const receivedData = JSON.parse(event.data) as SocketMessage;
        if (receivedData.name === 'sensor_hub' && typeof receivedData.value === 'object') {
          setHubState(receivedData.value as HubStateValue);
        }
      } catch (error) {
        console.error('Error parsing received message:', error);
      }
    };

    ws.current.onclose = () => {
      console.log('Disconnected from WebSocket server');
      ws.current = null;
    };

    ws.current.onerror = (error: Event) => {
      console.error('WebSocket error:', error);
    };

    if (isDev) {
      setHubState({
        device_count: 2,
        moisture_percent: 42,
        temperature_c: 22.5,
        humidity_percent: 61,
        devices: [
          {
            device_id: 'AA:BB:CC:DD:EE:FF',
            is_synced: true,
            moisture_percent: 42,
            temperature_c: 22.5,
            humidity_percent: 61,
            aqi: 30,
            tvoc_ppb: 88,
            eco2_ppm: 430
          },
          {
            device_id: '11:22:33:44:55:66',
            is_synced: true,
            moisture_percent: 68,
            humidity_percent: 54
          }
        ]
      });
    }

    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  const renderMetric = (label: string, value?: number | string) => {
    if (value === undefined || value === null || value === 'NaN') {
      return null;
    }

    return (
      <div key={label}>
        <strong>{label}:</strong> {String(value)}
      </div>
    );
  };

  return (
    <div className="App">
      <header className="App-header">
        <div className="hub-header">
          <h1>Sensor Hub</h1>
          <p>Tracked devices: {hubState.device_count ?? 0}</p>
        </div>

        <div className="hub-summary">
          {renderMetric('Moisture', hubState.moisture_percent !== undefined ? `${hubState.moisture_percent}%` : undefined)}
          {renderMetric('Temperature', hubState.temperature_c !== undefined ? `${hubState.temperature_c}°C` : undefined)}
          {renderMetric('Humidity', hubState.humidity_percent !== undefined ? `${hubState.humidity_percent}%` : undefined)}
          {renderMetric('AQI', hubState.aqi)}
          {renderMetric('TVOC', hubState.tvoc_ppb !== undefined ? `${hubState.tvoc_ppb} ppb` : undefined)}
          {renderMetric('eCO2', hubState.eco2_ppm !== undefined ? `${hubState.eco2_ppm} ppm` : undefined)}
        </div>

        <div className="device-list">
          {(hubState.devices ?? []).map((device, index) => (
            <div key={`${device.device_id ?? 'device'}-${index}`} className="device-card">
              <h3>{device.device_id ?? `Device ${index + 1}`}</h3>
              <div>{device.is_synced ? 'Synced' : 'Discovered'}</div>
              {renderMetric('Moisture', device.moisture_percent !== undefined ? `${device.moisture_percent}%` : undefined)}
              {renderMetric('Temperature', device.temperature_c !== undefined ? `${device.temperature_c}°C` : undefined)}
              {renderMetric('Humidity', device.humidity_percent !== undefined ? `${device.humidity_percent}%` : undefined)}
              {renderMetric('AQI', device.aqi)}
              {renderMetric('TVOC', device.tvoc_ppb !== undefined ? `${device.tvoc_ppb} ppb` : undefined)}
              {renderMetric('eCO2', device.eco2_ppm !== undefined ? `${device.eco2_ppm} ppm` : undefined)}
            </div>
          ))}
        </div>
      </header>
    </div>
  );
}

export default App;
