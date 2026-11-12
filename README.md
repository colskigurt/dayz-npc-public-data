# DayZ NPC — 현재 작업과 검증 상태

최신: [실제8개체 상태 대조 결과](reports/operations/lazy-crowd-eligibility-live8-20261112.md)

|항목|현재 판정|
|---|---|
|실제8개체 90초|최종 종결8/8, 전원 인식·2m 이동|
|캐시/원래 판정 대조|57,428회, 불일치0|
|행동 관찰|등반42·하강9·착지15|
|비용·화면 품질|이번 shadow에서 미측정|
|64개체·최종 채택|아직 미검증·미수용|

기존13개 PBO 변경 없이 격리 미션만 사용했다. 물리 검사·밀침·지속 등반·낙하·애니메이션 갱신율은 유지했다. 관측기 예상64/실제8 예외는 보고서에 명시했고 다음 seed는 수정했다.

다음 단일 작업: shadow 계산을 끈 동일 조건 기준/후보 전체 CPU 비교. 한 번의8/8을 모든 기능·64성능 성공으로 일반화하지 않는다.20ms p95는 목표다.

이전 [합성 국소 선별](reports/operations/lazy-crowd-eligibility-local-20261112.md)은 밀집 -13.71%/중간 -11.91%/희박 +0.10%였으며 전체 CPU 개선이 아니다. [현재64 CPU 분석](reports/operations/current-single64-cpu-attribution-20261112.md)의 Ray/Sphere inclusive17.84%도 self-time이 아니다.

[선정 원시 근거·후보 코드](data/lazy-crowd-eligibility-live8-20261112-evidence.txt) · [기계용 상태](api/public/status.json) · [외부 검증](verification/latest.json)

공개 JSON의 기존 schema 불일치는 미해결이며 HTTP·파싱 성공과 schema 전체 준수는 별개다. 기존 기록·고정 커밋은 보존한다.
