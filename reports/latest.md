# DayZ NPC — 현재 상태

최신 진단(2026-11-09):공식프로파일러 시간단위·평균 검증. NPC없는 작은진단이며 최적화 성공이 아닙니다.

|확인|결과|
|---|---|
|초기 resolution / average|100000 / false|
|resolution1000|ms 단위 반환 검증|
|평균 분모|sessionFrames49 (profiledFrames48와 다름)|
|부모/자식 시간|중첩 관찰·합산 금지|
|과거 NPC rawTime|당시 옵션 미기록이므로 확정 변환 보류|

[단위 진단 보고서](operations/enprofiler-units-20261109.md) · [공개 수치](../data/enprofiler-units-20261109.json)

NPC 주력 상태는 그대로입니다. [그래프변수 캐시](operations/graph-write-clean-cost64-screen2-20261109.md)는 OFF813.295→ON820.405ms/벽시계초로 미수용. [64개체 접촉 관측](operations/ground-progress-observation-sixtyfour-20261109.md)은 직접종착12/64·전체기능미수용, 내부Displace 솔버39.03%는 호출수 비율입니다. [후열 감속](operations/rear-pressure-shadow-20261109.md)은 보류. 셀 풀은 기존2쌍에서전체채택실패로재시험하지않습니다.

지속 등반·밀침·낙하·재등반 정책은 보존하며, 기능·시각·성능을 분리 판정합니다.20ms p95는 목표입니다.
