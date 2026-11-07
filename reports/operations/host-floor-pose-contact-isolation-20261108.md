# 384호스트 기본·본·접촉 비용 분리 진단

## 1. 작업 목표
현재 호스트를 유지한 상태에서 기본 실행, 본 조회, 원래 접촉 계산의 추가 비용을 구분. 최적화 후보나 실제 이동 기능 시험이 아니다.

## 2. 실제 수행
동일11PBO·64논리/320렌더/64receiver·고정 논리root·명시Idle0. AI/Drive/Crowd/frameFall OFF, 기존 transform Commit ON.
A는 호스트 유지, B는20Hz/개체4본·회전·임시배열 조회, C는20Hz 원래 SolveContacts. C결과는 probe 접촉이력만 갱신하고 root/heading/lift 이동에는 적용하지 않음. 지지 성공 강제·물리 삭제 없음.
한 클라이언트에서 A-B-C-C-B-A:40/70/100/130/160/190게임초부터15벽시계초씩.
첫 실행은2/6뒤 signed32 TickCount가 음수로 순환하자 수집기 양수 포맷 검증으로 실패. 원본 수집기와 부분CSV 보존. 격리 어댑터의 한 검증식을 signed integer로 수정하고 실제 양/음수 행을 확인한 뒤 동일 미션으로 재시험6/6 완료. 실패 실행을 완료 반복으로 합치지 않음.

## 3. 측정 결과
|조건|앞/뒤 CPU ms/벽시계초|평균|미션dt p50/p95/p99 ms|
|---|---:|---:|---:|
|A 호스트 유지|218.526 /230.017|224.272|16/31/31|
|B 본 조회|229.005 /233.307|231.156|16/31/31|
|C 원래 접촉 계산|292.704 /272.750|282.727|18/31/31|

기술적 평균 차이 B-A6.884ms/초, C-B51.570ms/초. 후자는 pose준비/배열/계수/probe이력/native질의를 포함하며 순수 질의 self-time이 아니다.
독립 프로세스1개, 조건당 관찰 구간2개. 미션dt는 클라이언트 Present가 아니다. 갱신 약50회/게임초,50FPS제한 조건.
probe76864/76864성공·실패0, 논리64root고정, 관찰320render+64receiver생존.6개 시계gate 통과(CPU읽기 최대4.685ms·clock읽기 최대26.740ms). 음수counter4580frame읽기 성공.
command/partGame0.99697~1.00206은 스크립트명령시계이며 실제 graph/표시시계로 해석하지 않음.
서버RPT 경계경고0. 앞선 이동OFF/ON 서버RPT도 재확인0:이번 서버CPU를 해당 로그쓰기 탓으로 설명할 근거 없음. 클라이언트 로그는 별개.
Present/GPU/실제 화면품질·기능 최종수용 미측정.

## 4. 원인 분석
384호스트를 모두 유지한 고정Idle장면은 기존 이동장면 약828ms/초보다 낮은 범위였다. 호스트 수만으로 전체 격차를 설명했다고 할 수 없음.
두 장면의 동작/request/작업량이 달라 차감해 AI비용 비율을 계산하지 않는다. C의 두 관찰292.70/272.75 차이도 있으므로 독립 반복이나 정확한 비용 분해로 표현하지 않음.
다음 우선순위는 이동 실행과 변환 갱신의 실제 소유자다. 기존 Commit은 unchanged transform을 생략하지만 이동시 여러 표시·수신체를 missionframe에서 갱신할 수 있다. 그 CPU 비중은 아직 미귀속이며 주기·보간 변경은 시작하지 않음.

## 5. 검증 수준
SINGLE_PROCESS_SIX_BLOCK_DIAGNOSTIC_ONLY.
분리 진단 및 수집기 signedcounter 오류 재현/수정 확인. 독립반복 성능·새 최적화·실제8/64 기능·시각·배포 수용 아님.20ms달성 아님.

## 6. 변경 사항
격리 미션/분석/측정제어와 수집기 어댑터만 추가. 기존 수집기·게임코드·PBO·그래프·운영설정 불변, 새 PBO/서명 대상 없음. 데스크톱 실행기 추가 없음.
mission SHA256 3a7305ddd9836069ddd8b8e6a69adc11200c40099324ef0eb630c207ded74a09
sampler adapter SHA256 53d9f47a23881bf0512733f6ebfd745881b054023c30cb0fb8e296f302ad35d0

## 7. 원시 증거
ANIM-host-floor-isolation1(중단2/6), ANIM-host-floor-isolation-retry2(완료6/6).
내부 capture0..5 CPU/receipt, probe/frame/partclock/gameplay/mixedhosts, warmup, analysis/decision/contract, samplerfixproof/diff/regression 및 부분CSV 보존. 저장 전 해시 검증. raw파일은 공개하지 않음.
서버 퇴역/detach/RequestExit 뒤 actualexit-1, 강제 종료 없음, 클라이언트 정상닫기.

## 8. 잔여 문제
다음 단일 산출물: 기존 현재CPU ETL에서 SetPosition/SetOrientation 및 이동 소유자의 한정 범위 귀속. 새 게임·전체Ghidra 초기화 없이 기존 분석 재사용.
스택/호출 경계가 부족하면 미확인으로 남기며 원인을 단정하지 않음. 새 보간/갱신주기·서버파트 삭제·기능복구 개발로 즉시 확대하지 않음.
full8/64 기능 실패·시각 최종검증·전체CPU 개선 목표는 미완료.
