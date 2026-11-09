# DayZ NPC Observatory — 현재 상태

2026-11-09: **컴파일된70개 ANM의 노드 헤더를 원입력과 교차 확인했다.** 반복 상수192채널이 모두 개수1인 필드와 대응해, 텍스트 중복 제거는 Workbench의 기존 축약과 겹칠 가능성이 높다. 단일 값 입력인데 필드가1이 아닌84개 예외와 실제 런타임flag/수신자 연결은 미확인이다. 자산·코드 수정과 새 게임 실행은 없다.

- [최신 분석·예외·판정 한계](reports/operations/anm-compiled-counts-review-20261109.md)
- [구조화 상태](data/anm-compiled-counts-review-20261109.json)
- [앞선 네이티브 샘플링 경계](reports/operations/animation-sampling-boundary-review-20261109.md)
- [1mm 허용 후보 비용 실패](reports/operations/foot-range-tolerance1-method-20261109.md)
- [보고서 전체](reports/operations/)

키축소나14클립 재컴파일은 보류한다. 다음은 현재 지원경로의 단일 로컬 보조파트 역할 대조 가능성을 준비하며, 실제 서버파트 제거/대규모 재설계는 하지 않는다.64체 비용과20ms p95 목표는 여전히 미해결이다.
