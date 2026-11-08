# 현재 상태 — 서버 비대표 갱신 정책 조사

| 항목 | 최신 근거와 판정 |
|---|---|
| 이번 시험 | 현재 네트워크8개체, 서버 비대표part1·3에 LOD1 한 번 지정 후 복원 |
| 실제 관측 범위 | 첫 NPC의5파트, 10,010행. 지정 대상은8NPC 총16호스트 |
| 일시 효과 | 지정 직후 대상 약10.43회/게임초, 대조 약20.86회/초 |
| 지속성 | 이후 안정 구간은 대상·대조 모두 약20.12회/초. 일회 지정 미수용 |
| 현재 내부 writer | 미추적. 과거 자동정책과 양립하지만 이번 확정 원인 아님 |
| 이번 기능·시각·성능 | 40초 진단으로 전체 경로 기능/클라이언트 시각/CPU·Present 수용 불가 |
| 다음 정책 | 주기적 LOD 유지 미구현. 역할·상태 복원/설정 오버헤드/동기 위험을 먼저 평가 |
| 20ms p95 목표 | 전체 최종 달성 미입증 |

기존 배포본과 물리 질의·대표·피해수신체·클라이언트 LOD는 유지했다. 발 광선 수치 동등성 기각도 유지하며 새 오차 기준은 적용하지 않았다. 시험 프로세스는 종료했다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/server-follower-lod-one-shot-20261109.md) · [직전 접촉 관측 한계](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/native-peer-contact-observation-inconclusive-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
