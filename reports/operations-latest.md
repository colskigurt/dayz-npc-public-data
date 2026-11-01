# 서버 충돌 경로 연결:mode1 평가와 별도 컨텍스트 포인터 초기화 구분

작성: 2026-11-01T14:47:56.131261+09:00

## 1. 작업 목표

서버 역할 시험의null 읽기가 어느 평가 경로에서 발생했는지 확인하고, 포인터의 초기화 출처를 좁힌다. 추가 충돌 재현·새 시계 보정 없이 기존 실패 증거를 사용한다.

## 2. 실제 수행

실제 서버 SHA와1파트 실행의 저장된 모듈 기준 주소/stack을 재대조했다. fault가 들어 있는 PE unwind의CHAININFO 두 단계를 따라 원래 함수 시작0x140a52400을 복구했다. 기존0xA52EC4는 함수 전체 진입이 아닌 chained 구간이었다.

서버 스케줄러·평가 함수·하류 애니메이션 처리의 직접 call과 보고된 return address를 연결했다. 이어 서버 Creature의 컨텍스트 생성·저장 코드와 factory를 제한적으로 디코드했다. 이름이 비슷한 공개 DayZCreatureAnimInterface wrapper의 RTTI/생성자도 별도로 대조해 섣불리 동일 객체로 연결하지 않았다. 새 게임·클라이언트·서버 실행,메모리 쓰기,PBO 변경은 없다.

## 3. 측정 결과

chained unwind3개, 인접 직접 call 연결3개를 확인했다. 보고된 return RVA0xAAAB8F는 서버 scheduler의mode1 분기에서0x140AAAC90을 호출한 직후 주소와 일치한다. 그 하류는0x140A53D30 →0x140A52400이다. 이는 보고된 stack과 실제 명령을 연결한 근거이며 새 mode 필드 실측은 아니다.

중요하게 서버의mode 필드는+0x5F8, 컨텍스트는+0x5B0이다. 클라이언트의+0x668/+0x620을 서버에 그대로 적용하면 안 된다. 이번 분석은 실제 서버 명령으로 이 차이를 확인했다.

Creature 생성자는 entity를factory의r8로 전달하고 반환값을entity+0x5B0에 저장한다. factory0x140A45C40은entity 인자를context+0에 저장하고context+8은0으로 초기화한다. 따라서 crash의context+8을 핵심entity 포인터context+0과 혼동할 수 없다. 어떤 후속 코드가+8을 채워야 하는지는 아직 미확인이다.

하류 함수에는 Update Anim Job 문자열과 애니메이션 컨텍스트 호출이 연결된다. 기존 서버 실패는 하류 native 평가 안의null 읽기까지 좁혀졌다. 이 자체로 AI template·graph·model 중 원인을 확정하지 않는다.

새 p50/p95/p99·CPU/GPU·표시·시각 품질·8/64 기능 회귀는 미측정이다. 이전1파트 실패는 pose0으로 유지하며20ms p95 목표 달성을 주장하지 않는다.

## 4. 원인 분석

확인: 기존 서버1파트 stack은 별도mode1 평가 경로와 연결된다. 현재 입력 시간을 직접 평가하는 다른 분기를 사용했다는 이유만으로 현재 자산의 서버 호환성이나 부드러움을 기대할 수 없다. 실제로 하류에서 실패했다.

확인: core entity 인자는 factory에 전달되고+0에 저장되는 경로가 있다. null인+8은 다른 초기화 필드다. 그러므로 entity 소유자 전체가 빠졌다고 단정해 포인터를 주입하는 수정은 부적절하다. 공개 AnimInterface wrapper는 다른 생성자·레이아웃으로 확인돼 raw context의 지원 setter라고 가정할 근거가 없다.

가설/미확인: secondary 포인터가 모델·렌더·애니메이션 연결 중 어느 객체인지, 어느 코드가 이를 채우고 왜 이번 서버 fixture에서는null인지. AI 초기화 문제가 단독 원인이라는 설명은 아직 입증되지 않았다. 기록된 stack은 독립적으로 완전 unwind한 덤프가 아니어서 동적 증명의 강도를 제한한다.

## 5. 검증 수준

서버 바이너리의 chained 진입점·분기/직접call·컨텍스트 생성 대조는 제한된 진단 성공이다. 메모리 필드 실측·지원 API 확보·수정·새 회귀·화면 품질·성능·최종 수용은 미완료다. static RTTI 후보를 실제 객체 타입 증명으로 취급하지 않는다.

## 6. 변경 사항

격리 research 묶음에 읽기 전용 분석 스크립트와 소규모 JSON을 추가했다. 기존 게임 코드·PBO·ANM·개인키·운영 서버·9350 후보는 보존했다. 새 PBO가 없어 새 서명 대상은 없다. 직전3기존PBO 검증 성공 기록은 유지하되 이번 새 검증으로 중복 주장하지 않는다.

## 7. 원시 증거

animation-server-owner-20261101-1442. owner-analysis/graph-context/context-producer/factory/interface-link/context-types와 재현 스크립트를 연결한다. 서버 SHA ee1add8bb6ded0b6e913f31d62c93d1326d5a44106d558bbd3d19adb78b73bb1,기존1파트crash/launch 해시를 대조한다. 공개는 정규화된 RVA·선정 함수/명령·정적 필드 관계이며 PID·ASLR 기준 주소·실제 command line·원시 전체 로그는 제외한다.

## 8. 잔여 문제

추가 서버 시험보다 context+8의 후속 producer/attachment를 먼저 확인한다. 지원 접점이 없다면 native-only/서버 호환성 한계를 명시하고 오프라인 시계 손실 및64비용 분석과 분리한다. mode1을 강제로 선택하거나 포인터를 쓰는 수정은 하지 않는다.

후속 실험이 필요하면 secondary 초기화 가설을 구분할 최소 대조를 정하고 한 요인만 변경한다. 이번 정적 연결을 성능 최적화나 시각 품질 개선으로 발표하지 않는다. 정상 밀침·낙하·착지·FOLLOW·재등반의 기존 기능 증거와 전체 목표는 보존한다.

선정 공개 본문. 로컬 보고서 SHA-256: `5b13b5af5aaf58c0f20d3e1d64ccacbeedbb5a4c4d4ce0cdc3bdc2be46f7f3f1`. 원시 전체가 아니다.
