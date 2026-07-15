# 2048-bot

Last updated July 14, 2026

## Performance

| Search depth | 1024  | 2048  | 4096  | 8192   | Total tests |
|--------------|-------|-------|-------|--------|-------------|
| 1            | 96.9% | 93.2% | 87.9% | 44.0%  | 22172       |
| 2            | 98.9% | 97.9% | 97.0% | 70.0%  | 1185        |

## Algorithm

The bot uses expectimax search with a multi-stage n-tuple network for evaluation. Tuples are taken from *Multistage Temporal Difference Learning for 2048-Like Games by Yeh et al.* and are trained using a lot games.