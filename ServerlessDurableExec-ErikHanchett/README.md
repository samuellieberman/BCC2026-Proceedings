# Make Your Serverless Functions Do More with Durable Executions

## Erik Hanchett

### Boise Code Camp 2026 - May 2nd, 2026

Serverless architectures are great to start with. But when your application needs long-running, multi-step processing with built-in resilience, a simple function won't cut it. You need durable executions and stateful processing.

This talk covers fault-tolerant, durable execution patterns: checkpointing, replay, and how step functions work. I'll walk through an order system that authorizes a payment, waits for manual approval, then captures or cancels the charge. We'll dig into the code and compare this to traditional serverless functions.

You'll leave understanding how durable executions work and when to use them.
