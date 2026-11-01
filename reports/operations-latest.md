# native LOD2 덮어쓰기 caller 포착: 공개 LOD0 이후 interval 재설정

작성: 2026-11-01T15:44:21.909222+09:00

## 1. 작업 목표

생성 직후 LOD0을 지정해도 50ms 간격이 200ms로 돌아가는 원인을 실제 writer와 caller 수준에서 확인한다. 기존 오프라인 mode0 잔여시간 손실 진단, 64개체 기능 회귀 및 비용 실험과 구분한다.

## 2. 실제 수행

기존과 동일한 1파트 물리 리더, 고정 run, AI·루트 이동 OFF, 플레이어 없는 격리 loopback 서버를 한 번 실행했다. 기존 453D/0C6E/B8FB PBO와 서버 EE1A를 유지했다. 생성 시 공개 UpdateSimulationPrecision(0)을 한 번 호출하는 init 소스도 이전 B와 동일 해시다.

생성 전에 기존 Python Frida 관찰기를 연결해 공개 precision setter, 내부 LOD dispatcher, 실제·요청 interval writer 및 선택 개체의 evaluator를 관찰했다. 함수 바이트 가드 5개를 확인했고 수치·포인터·모드를 변경하지 않았다. 관찰 훅 자체는 코드 계측이며 순수 읽기 전용 프로세스 접근이라고 부르지 않는다. 이 실행은 성능 비교에서 제외했다. 관찰은 READY 뒤 약12초로 제한하고 훅을 비활성화한 뒤 소유 서버 종료까지 유지했다.

해당 실행 파일의 해시를 검증하고 Capstone으로 포착한 반환 주소 주변 명령, RTTI vtable 및 getter의 직접 분기를 대조했다. REA 런타임 Evidence가 아니라 기존 Python Frida 및 제한적 정적 분석 결과다.

## 3. 측정 결과

공개 LOD0 호출1회, 내부 dispatcher LOD0 1회·LOD2 6회, 실제 interval writer7회·요청 writer7회, evaluator90표본을 기록했다. 관찰기 오류0·스크립트 오류0·수치 override0이다. DONE 이후 계획된 소유 프로세스 종료를 확인했으며 정상 종료로 기록하지 않는다.

공개 호출 전후 requested50/50ms·actual50/200ms였다. 첫 LOD2 기록은 공개 호출 약1871ms 뒤였으며 actual writer가200/500ms로, requested writer가200/200ms로 차례로 기록했다. 시각은 Date.now 기반 관측 순서로 정밀 frame-time이 아니다. 이전 probe의173ms와 이번1871ms를 고정 전환 지연으로 일반화하지 않는다. 반복 호출의 전체 시각은 값 변화가 없을 때 발췌되지 않으므로6회만으로 정확한 주기를 산출하지 않는다.

LOD2 호출 return RVA는0x5308eb다. 바로 앞0x1405308e6의 call은 내부 dispatcher0x140aab460으로 연결된다. 같은 수신자에 대해 call [rax+0xb10]의 반환 EAX를 EDX로 전달하고 R8B=1로 요청 interval 갱신을 허용하는 명령을 확인했다. 별도 공개 API 재호출 없이 내부 경로가 값을 다시 썼다는 근거다.

이번에는 8/64 회귀, 화면·GPU·Present, frame-time p50/p95/p99, native graph clock, 할당/GC를 측정하지 않았다.20ms p95 달성 및 애니메이션 시각 품질 개선은 미수용이다.

## 4. 원인 분석

확인된 사실: 생성 시 공개 API 한 번으로 지정한 간격은 이후 native LOD2 정책 호출에 의해 다시 설정된다. 따라서 단순 미호출 가설보다 내부 정책 덮어쓰기라는 설명이 직접 근거를 얻었다. 해당 caller 경로의 가상 슬롯은+0xb10이다.

정적 후보: RTTI의 DayZAnimal 기본 vtable에서 이 슬롯은0x140aad120으로 연결된다. 이 getter는 flag+0x61e가 있으면-1을 반환하고, 그렇지 않으면 입력 XMM1을6400/40000과 비교해0/1/2를 선택한다. 이어 연결 데이터의+0x35 및+0xf4/f5 조건이 참이면 최대1로 제한한다. 그러나 실제 개체 vtable과 getter 입력은 이번 관찰에서 포착하지 않았다. 따라서 이 정적 후보를 실제 실행 selector로 확정하거나 입력을 거리 제곱으로 이름 붙이지 않는다.

SetRequiredSimulation의 확인된 native handler는+0x566을 쓴다. getter의+0x61e와 다르므로 이 API가 자동 LOD를 막는다는 가설을 현재 근거로 채택하지 않는다. DisableSimulation 역시 문맥의 활성 상태를 건드리는 경로여서 시간 정책 우회책으로 사용하지 않는다. flag+0x61e writer 후보는 발견했지만 완전한 의미와 호출 조건은 미확정이다.

simulation-control-slots.json의+0x6e8 선형 디코드는 tail jump 뒤 이웃 함수까지 이어졌다. 소유권 분석에는0x140aa4745의 jump까지 사용하고 이후 명령은 제외한다. offset byte-pattern 검색은 leaf·인라인·다른 주소 계산을 놓칠 수 있어 전체 참조 부재 증거로 쓰지 않는다.

## 5. 검증 수준

native interval 덮어쓰기와 직접 caller 연결은 진단 성공이다. 실제 selector 입력·정책 제어 API는 미확정이다. 기능 회귀 통과, 반복 시각 검증, 성능 개선 및 최종 수용을 선언하지 않는다. 기존 mode0 CPU/native/화면 시간 불일치는 여전히 별도로 남아 있다.

## 6. 변경 사항

격리 관찰기·정적 분석 파생본·작은 서버 profile만 추가했다. 현행 게임 코드·PBO·ANM·운영 서버·9350 후보를 변경하지 않았다. 기존3개 PBO 서명과 해시 검증은 성공했다. 새 PBO가 없어 새 서명 대상은 없다. 영상이나 대용량 dump는 생성하지 않았다.

## 7. 원시 증거

실행 ID animation-precision-writer-20261101-1530. writer-observer.json/js, writer-callers.json, complete-getters.json, simulation-control-bindings.json, flag-field-references.json, simulation-control-slots.json 및 서버 launch/profile을 연결한다. 원본은 로컬에 보존하며 공개본은 선정 이벤트·caller 문맥·정적 getter로 제한한다. PID·ASLR base·실제 명령줄·전체 원시 로그·게임 자산·키는 공개하지 않는다. 실행 및 관찰기 종료, JSON 파싱, 기존 해시와 파일 안정성을 확인한 뒤 보고서를 저장한다.

## 8. 잔여 문제

다음 실험은 같은 1파트 조건에서 실제 vtable, slot+0xb10 getter 입력·출력, flag+0x61e와 입력값 생산자를 제한적으로 관찰하는 것이다. 실제 Animal 경로가 맞으면 정책 입력과 지원 API 연결을 검토하고, 다르면 실제 override의 소유자부터 수정한다. 현재 서버와 기존 관찰기로 진행 가능하며 이 단계에 클라이언트나 새 영상은 필요하지 않다.

지원 제어가 증명되기 전에는 native flag 변경, 매 틱 LOD0 강제 재호출, 새 보간 구현을 보류한다. 해시·서명 불일치, crash, 관찰 오류 또는 대상 범위 이탈 시 후속 실행을 멈추고 증거를 보존한다. 화면 품질과64 비용은 별도 통제 실험에서 확인해야 한다.

선정 공개 본문. 로컬 보고서 SHA-256: `fe3d4a315d98fb5cfe109e2ec94ed4c48b9db58e33e3c71710dc5e705f591d52`. 원시 전체가 아니다.
