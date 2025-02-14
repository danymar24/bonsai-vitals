import React, { useEffect, useRef, useState } from 'react';
import './App.css';
import { SocketMessage } from './types';

const isDev = process.env.NODE_ENV === 'development';

function App() {
  const [message, setMessage] = useState('');
  const [receivedMessages, setReceivedMessages] = useState<any[]>([]);
  const [moisture, setMoisture] = useState<SocketMessage>();
  const ws = useRef<WebSocket | null>(null); // Use a ref to store the WebSocket instance

  useEffect(() => {
    // Replace with your WebSocket server URL
    ws.current = new WebSocket(`ws://${window.location.host}/ws`);

    ws.current.onopen = () => {
      console.log('Connected to WebSocket server');
      // Optionally send an initial message upon connection
      ws?.current?.send(JSON.stringify({ type: 'join', data: 'Client joined' }));
    };

    ws.current.onmessage = (event: MessageEvent) => {
      try {
        const receivedData = JSON.parse(event.data); // Parse received JSON
        const messageData: SocketMessage = receivedData;
        console.log(event.data.name);
        console.log(messageData);
        if(messageData.name === 'moisture') {
          setMoisture(messageData);
        }
        setReceivedMessages((prevMessages: any) => [...prevMessages, receivedData]);
      } catch (error) {
        console.error('Error parsing received message:', error);
        setReceivedMessages((prevMessages: any) => [...prevMessages, event.data]); // Fallback to raw data
      }
    };

    ws.current.onclose = () => {
      console.log('Disconnected from WebSocket server');
      ws.current = null; // Clear the WebSocket instance
    };

    ws.current.onerror = (error: Event) => {
      console.error('WebSocket error:', error);
    };

    if(isDev) {
      setMoisture({
        name: 'moisture',
        type: 'percentage',
        value: '33'
      })
    }

    // Clean up on unmount to prevent memory leaks
    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };

  }, []); // Empty dependency array ensures this runs only once on mount and unmount

  const sendMessage = (newMessage: string) => {
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      const messageToSend = JSON.stringify({ type: 'message', data: newMessage });
      ws.current.send(messageToSend);
      setMessage(''); // Clear the input field
    } else {
      console.error('WebSocket connection is not open.');
    }
  };

  return (
    <div className="App">
      <header className="App-header">
        <div className='moisture'>
          <p>Moisture: {moisture?.value}%</p>
        </div>
        {/* <div>
        {receivedMessages.map((msg, index) => (
          <div key={index}>
            {msg.type === 'message' ? <span>Client: {msg.data}</span> : <pre>{JSON.stringify(msg, null, 2)}</pre>}
          </div>
        ))}
        </div>
        <input type="text" value={message} onChange={(e) => setMessage(e.target.value)} />
        <button onClick={() => sendMessage(message)}>Send</button> */}
      </header>
    </div>
  );
}

export default App;
