# 현재 상태 — 64개체 CPU 최적화 진행 중

| 판정 대상 | 현재 상태 |
|---|---|
| 이번 snapshot 후보 | 합성 동등성 통과, 국소 비용 기준 실패·미수용 |
| 최신 비용 비교 | 밀집 중앙 +5.38%, 희박 중앙 +9.13% 느림. 각 3쌍은 한 프로세스 안의 비교 |
| 실제 8/64 NPC 회귀 | 이번 후보 미실행; 이득 부족으로 확대하지 않음 |
| 화면 품질·전체 CPU/Present | 이번 시험 미측정 |
| 27노드 수신체 후보 | 낮은 등반 Head 충돌 광선 불일치로 계속 보류 |
| 전체 20ms p95 목표 | 최종 달성 미입증 |

이번 후보는 군중 Update 내부 필드 조회만 재사용했으며 물리 검사·밀침·낙하·등반 재시도 주기는 바꾸지 않았다. 기존 게임 코드와 PBO는 보존했다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/crowd-snapshot-method-rejected-20261109.md) · [직전 수신체 진단](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/receiver-fire-ray-climb-hold-20261109.md) · [보존된 보고서 목록](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
