# DayZ NPC — 현재 작업과 검증 상태

최신: [군집 상태 판정 재사용의 국소 선별 결과](reports/operations/lazy-crowd-eligibility-local-20261112.md)

|항목|현재 판정|
|---|---|
|합성 512 상태 및 경계 검사|불일치 0, 국소 gate 통과|
|군집 Update 중앙값 변화|밀집 -13.71%, 중간 -11.91%, 희박 +0.10%|
|반복 범위|한 프로세스, 조건별 3쌍; 독립 3실행 아님|
|실제 8/64 기능·화면·전체 CPU|이번 후보 미측정, 미수용|
|20ms p95|목표, 달성 주장 없음|

변경은 격리 미션만. 물리 질의·지속 등반·힘 계산 순서·애니메이션 갱신율을 줄이지 않았다. 기존 PBO 변경 없음. 중간 배치 첫 블록 편차를 포함한 전체 결과를 보존했다.

다음 단일 작업: opt-in 8개체 shadow/ON 검증 후 전체 CPU 기여가 있는지 판단. 국소 비율을 서버 전체 개선으로 환산하지 않는다.

이전 [현재 단일 렌더 64 CPU 분석](reports/operations/current-single64-cpu-attribution-20261112.md): Ray/Sphere 포함 표본 17.84%, stack 결손26.13%, 완전 분해 아님. 바닐라 내부 질의 수·접촉 재사용은 아직 미확인.

[선정 근거](data/lazy-crowd-eligibility-local-20261112-evidence.txt) · [기계용 상태](api/public/status.json) · [HTTP 검증](verification/latest.json)

공개 JSON의 기존 schema 불일치는 아직 미해결이다. HTTP/파싱 성공과 schema 전체 준수는 별개다. 원시 자료·과거 보고서·이전 고정 커밋은 보존한다.
