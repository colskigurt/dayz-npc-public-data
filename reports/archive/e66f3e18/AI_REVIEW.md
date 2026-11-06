# DayZ NPC 현재 상태

최신: [SupportFast 검사량 회계 검토](reports/operations/supportfast-query-accounting-review-20261107.md). 추가 게임 실행0회.

| 확인 항목 | 판정 |
|---|---|
|8개체 기능|8/8,낙하9/복귀9;별도 통과|
|광선 감소|첫쌍58.23%|
|추가 지지sweep 포함 주요 호출량|게임초당29.56~30.17% 감소 범위;모든 질의 아님|
|실제CPU|첫쌍5.19% 증가;외부 부하 미통제·인과 불명|
|64비용 비교|4수집/3구간검증/1제외,완전1쌍;3쌍 미완료|
|성능·시각·전체20ms|최종 미수용|

호출량 감소를 CPU 절감으로 대체하지 않는다. 추가 안전sweep을 제거하거나 기존에 기각한 Drive/Displace 병합을 다시 적용하지 않는다. 전용 측정 시간 확보 전 새 클라이언트 실행을 보류한다.

[공개 JSON](api/public/supportfast-query-accounting-review-20261107.json) · [원래 비용 보고서](reports/operations/supportfast64-cost-interim-20261107.md)
