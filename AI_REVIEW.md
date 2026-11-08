# 외부 AI 검토 안내

# DayZ NPC — 현재 상태
갱신: 2026-11-08T00:41:07.731Z

| 판정 | 현재 결과 |
|---|---|
| 현재 엔진14클립 CPU 포즈 | 단일 진단 통과 |
| 동일조건8개체 제한 통합 | 양쪽 생존·인식·이동8/8, 상태기록 정합성 오류0 |
| 전체 기능 | 미수용:5파트 유효최종6/8,4파트5/8 |
| 미해결 | 4파트 종료시EGRESS 약6.662초,회귀원인미확정 |
| 이번 시각·CPU·Present | 미측정 |
|20ms p95|목표·미달성|

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-eight-mixed-20261108.md)

동일14PBO·동일mission,layout플래그만 변경. 조건당1회이며 반복검증이 아니다. 사전검사5고정으로 실패한첫4파트시도는 보존·비교제외했다. 제한 통합 통과는 전체 기능·회귀없음·배포 승인과 다르다.

[직전14클립 진단·과거 기록 정정](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-fullclip-pose-correction-20261108.md)

다음:기존 실패구간 접촉입력 차이를 제한 검토 후 전체CPU 선별가치 판단. 기능복구로 주력을 전환하지 않는다.
