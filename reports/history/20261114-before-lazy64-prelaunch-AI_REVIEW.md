# DayZ NPC — 현재 비용 최적화와 검증 상태

최신 정적 조사: [Dark Horde 내부 코드·LG 다운로드 결과](reports/operations/community-horde-package-review-20261114.md). Dark Horde V1.30.t23 패키지/Enforce 코드 확인, LG는 공식 Steam File Not Found(9)로 내부 미확인. 근거리 대체 솔버 미발견, 새 게임·성능 측정0회.

최근 성능 실험: [제외 관측 수 정정·재측정 준비](reports/operations/lazy-crowd-eligibility-exclusion-correction-20261112.md)

|독립 비교|서버 CPU 평균 변화|판정|
|---|---:|---|
|1: OFF→ON|-5.015%|유효, 단독 채택 불가|
|2: ON→OFF|-1.539%|유효|
|3: OFF→ON|-6.701%|측정 중 백그라운드 전환, 제외|

**유효2쌍·제외1쌍이므로 반복 검증 미완료·후보 미수용.**유효 두 값의 요약 중앙 -3.277%를 최종 개선율로 보지 않는다. 세 번째 유리한 수치는 유효 반복에 포함하지 않는다.

세 번째 기준의 CPU 구간 상태 관측26/28회가 비활성이었고, 전체 클라이언트 기록에는 비활성992프레임이 있었다. 누가 포커스를 바꿨는지는 모른다. 유효3쌍 기준을 완화하지 않는다.

정정: 원시 기록은26/28이며 앞선 서술의25/28을 바로잡았다. 제외 판정은 그대로다. 재측정 도우미는 합성9입력·문법 검사만 통과했고 실제 게임 종료 경로는 미시험이다. 새 NPC 실행0회.

다음: 전용 foreground 시간 확인 후 제외된 쌍의 양쪽 조건을 새로 측정. 현재 새 게임 실행은 보류 중이다. 기존 PBO·자산·원시 기록 보존, 신규 PBO 없음. 정상 밀침·낙하·착지·추적·재등반은 유지 대상이며 기능·Present·화면 최종 판정은 미완료.20ms p95는 목표다.

[CPU 원시 필드 투영·자체 코드·hash](data/lazy-crowd-eligibility-repeat-exclusion-20261112-evidence.txt) · [제외 판정](data/lazy-crowd-eligibility-repeat-exclusion-20261112-exclusion.json) · [기계용 상태](api/public/status.json) · [외부 검증](verification/latest.json)

이전 [8개체 대조](reports/operations/lazy-crowd-eligibility-live8-20261112.md)57428회 불일치0/종결8/8은64성능 수용을 대신하지 않는다. 공개 JSON의 기존 schema 불일치는 미해결이며 HTTP·파싱 성공과 전체 schema 준수는 별개다.
