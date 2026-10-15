/*

Producer
    ↓
Message Queue
    ↓
Consumer / Worker
    ↓
Success? ── Yes → ACK
    │
    No
    ↓
  Retry
    ↓
Max retries reached?
    ↓
Dead Letter Queue

*/