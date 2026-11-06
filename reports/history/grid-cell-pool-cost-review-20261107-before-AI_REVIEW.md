# DayZ NPC 현재 상태

- 최신: [lane-spacing-eight-pair1-20261107](reports/operations/lane-spacing-eight-pair1-20261107.md)
- 8개체 레인 간격 0.8→0.95m 단일 비교: 양 조건 8/8 완료, 낙하·착지 FOLLOW 복귀 9/9.
- 접촉 쌍 -7.18%, 전체 solve +8.67%, 완료 시간 55.9148→61.7306초. **비용 개선 미입증, 후보 기본 OFF·미수용**.
- CPU/Present p50/p95/p99 미측정, 64개체 미실행, 시각 품질 미판정. 기존 64개체 성능 목표·애니메이션 최종 수용은 미완료.
- 다음: 실제 추가 solve 발생 조건의 국소 변경 가능성 검토. 기능 개발로 확대하지 않음.
- [공개 JSON](api/public/lane-spacing-eight-pair1.json)

기존 상세 기록: [AI_REVIEW.md 보존본](reports/history/lane-spacing-eight-pair1-20261107-before-AI_REVIEW.md)
