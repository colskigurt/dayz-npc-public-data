# 현재 상태 — 64개체 CPU 최적화 미완료

| 항목 | 판정 |
|---|---|
| 수신체 축소 후보 | 기존 Head 광선 불일치 재현, 채택 보류 |
| 이번 요인 분리 | 첫 대조 14/1,980 불일치. 모델만 축소한 교차 조합은 생성 중 접근 위반. 세 번째 조합 미실행 |
| 내부 원인 | 미확정. 모델/애니메이션 어느 쪽 원인인지 단정 불가 |
| 군중 snapshot 후보 | 앞선 합성 동등성 통과, 국소 비용 증가로 기각 |
| 실제 8/64 NPC·시각·비용 | 이번 요인 시험에서 미측정. 크래시 실행은 성능 평가에 사용하지 않음 |
| 20ms p95 | 전체 목표 달성 미입증 |

이번 격리 시험은 종료됐고 기존 운영본은 보존했다. 새 설정 PBO의 서명과 검증은 성공했으나 런타임·최종 수용을 뜻하지 않는다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/receiver-factor-climb-inconclusive-20261109.md) · [snapshot 비용 기각](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/crowd-snapshot-method-rejected-20261109.md) · [수신체 원본/후보 대조](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/receiver-fire-ray-climb-hold-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
