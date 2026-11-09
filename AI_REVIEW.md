# DayZ NPC Observatory — 현재 상태

2026-11-09: **명시적 클라이언트 로컬 명령도 미수용.** 설치SDK에서공개StartCommand계약을확인하고현재로컬파트에서물리본체·인터페이스·변수바인딩·실제명령설치동일성을확인했다. 그러나1000활성표본에서기준본은999/999전환변화,로컬CPU3본변화0·명령콜백0이었다. 퇴역요청8초뒤에도명령이붙어있어객체를직접삭제하지않고소유클라이언트를종료했다.

- [최신 상세 보고서](reports/operations/client-local-explicit-start-20261109.md)
- [구조화 결과](data/client-local-explicit-start-20261109.json)
- [앞선 자동초기화 경계](reports/operations/client-local-role-capability-20261109.md)
- [1mm 후보 비용 실패](reports/operations/foot-range-tolerance1-method-20261109.md)
- [기존 보고서](reports/operations/)

전체로컬애니메이션불가능판정이나엔진버그확정은아니다. 서버5파트는유지,8→64확대없음. compile3 private소멸자오류/강제정리와미실행network3을보존했다. build3/4진단PBO서명검증성공,원본PBO불변. CPU/실제렌더품질미측정,20ms p95미달성.

명령설치성공과콜백실행·포즈·안전퇴역은별개다. 이번결과를성능성공이나실제화면정지증명으로해석하지않는다. 무근거AI/wake/메모리플래그변경으로확대하지않는다.
