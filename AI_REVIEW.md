# 현재 상태 — 64개체 CPU 최적화 계속

| 대상 | 현재 판정 |
|---|---|
| 최신 호스트 접촉 관측 | 8개체·표시40호스트·9,280행 활성. 네이티브 접촉 이벤트 전달0, 양성 보정 없음 |
| 해석 한계 | 네이티브 충돌 검사/비용0을 뜻하지 않음. 이벤트 경로로 비용 판별 미확정 |
| 선택적 호스트 pair-block | 근거 부족으로 아직 미구현 |
| 이번 기능 범위 | 목표 인식/2m 이동8/8. 최종유효4/8, 전체 기능 회귀 통과 아님 |
| 이번 시각·전체 CPU·Present | 미측정 |
| 이전 후보 | 수신체 축소는 충돌 불일치로 보류, snapshot은 국소 비용 증가로 기각 |
| 20ms p95 | 전체 시나리오 최종 달성 미입증 |

현재 배포본과 정상 밀침·낙하·착지·추적·재등반 코드는 유지했다. 이번 관측 모듈은 서명/검증됐지만 최적화 성공을 의미하지 않는다. 시험 서버·클라이언트 종료를 확인했다. 기존 검증 결과를 이번 제한적 관측 결과로 대체하지 않는다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/native-peer-contact-observation-inconclusive-20261109.md) · [수신체 요인 분리 한계](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/receiver-factor-climb-inconclusive-20261109.md) · [snapshot 비용 기각](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/crowd-snapshot-method-rejected-20261109.md) · [보존된 전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
