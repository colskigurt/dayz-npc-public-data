# DayZ NPC 현재 상태

- 최신 비용 비교: [grid-cell-pool-cost-review-20261107](reports/operations/grid-cell-pool-cost-review-20261107.md)
- 64개체 OFF/ON 2쌍 비교. 관찰 제한 단독 OFF1회 및 마지막 ON 미측정 크래시 별도 보존. 후보 미수용.
- presentP95 중앙값 +13.16%, 2쌍 모두 감소=False; cpuBusyP95 중앙값 +13.21%, 2쌍 모두 감소=False; serverCpuPerWallSecond 중앙값 -5.92%, 2쌍 모두 감소=True; serverCpuPerGameSecond 중앙값 -6.04%, 2쌍 모두 감소=True
- 전체 기능·시각 품질·프로젝트 20ms p95는 별도 미수용. 새 no-op 후보는 제거 가능 호출 상한이 작아 미구현.
- [공개 JSON](api/public/grid-cell-pool-cost-review.json)

기존 상세 기록: [AI_REVIEW.md 보존본](reports/history/grid-cell-pool-cost-review-20261107-before-AI_REVIEW.md)
