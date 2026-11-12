# DayZ NPC — 현재 비용 최적화와 검증 상태

최신: [64개체 전체 CPU 첫 비교](reports/operations/lazy-crowd-eligibility-cost64-pilot-20261112.md)

|항목|기준|후보|
|---|---:|---:|
|전체 서버 CPU 평균(ms/벽시계초)|625.46|594.09|
|미션 dt p95(ms, Present 아님)|33.00|32.05|
|생존·인식·2m 이동|64/64|64/64|
|90초 종료 valid/incomplete/state/fall|5/57/1/1|12/47/3/2|

**전체 CPU -5.015%는 독립1쌍의 관측이며 미수용이다.**5% 선별선 여유0.015%p뿐이다. 발 Ray/게임초+1.55%, 관절 Sweep+1.64%로 검사 생략에 의한 감소가 아니다. 미완료 상태와 궤적이 달라 순수 효과·기능 회귀 통과를 확정하지 않는다. 정상 낙하·복귀는 유지 대상이다.

다음 단일 산출물: ON→baseline 역순 독립 비교. 반복·기능·Present·시각 품질이 남아 있다.20ms p95는 목표다.

이전 [8개체 상태 대조](reports/operations/lazy-crowd-eligibility-live8-20261112.md)는57428회 불일치0/종결8/8이지만64기능 수용을 대신하지 않는다. 기존 PBO·자산·원시 기록은 보존했다. 신규 PBO 없음.

[CPU31샘플씩·후보 코드·원시 파일 hash](data/lazy-crowd-eligibility-cost64-pilot-20261112-evidence.txt) · [기계용 상태](api/public/status.json) · [외부 검증](verification/latest.json)

공개 JSON의 기존 schema 불일치는 미해결. HTTP·파싱 성공과 schema 전체 준수는 별개다.
