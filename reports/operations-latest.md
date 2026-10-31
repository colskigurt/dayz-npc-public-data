# 애니메이션 공개 제어 후보 조사: 빈 속도 헬퍼와 시각 컴포넌트 가드

작성: 2026-10-31T12:55:52.850555+09:00

## 1. 작업 목표

확인된 애니메이션 시간 손실을 지원되는 API로 제어할 경로가 있는지 대조한다. 실패한 일반 Animate 경로와 강제LOD를 반복하지 않고, 아직 조사하지 않은 공개 선언의 실제 실행본 구현을 확인한다.

## 2. 실제 수행

설치 scripts.pbo의 3056개 C 스크립트를 읽어 proto native와 animation/skeleton/simulation/bone 문자열을 포함하는 선언104개를 선정했다. 이는 정규식 조사이며 모든 API를 의미론적으로 분석한 것은 아니다. World.c 한 파일의 비UTF8 바이트는 Latin1로 읽었고 ASCII 식별자 검색 범위를 기록했다. 전체 자료를 새 폴더에 재복사하지 않았다.

DebugOverrideAnimationSpeed·PreAnim_SetFloat와 전역 SetAnimFrame·SetAnimSlot의 리터럴/등록 ABI를 실제 EXE에서 대조했다. 인스턴스 등록과 전역 등록의 인자 위치가 달라 별도 추적했다. .pdata 또는 첫 return 경계의 Capstone 명령을 읽었으며 실제 호출·후킹·PBO·미션 변경은 없었다.

DebugOverrideAnimationSpeed가 호출하는 정확한3바이트 헬퍼를 출처 hash/원래 파일 offset과 함께 추출했다. REA에서 이 raw x86_64 조각을 열려 했으나 Windows Ghidra P0가 해당 raw 형식을 지원하지 않아 거절됐다. 이것은 사용자 승인 거절이 아닌 공급자 형식 제한이다. 이전 전체 EXE 분석의900초 timeout을 반복하지 않았다. REA의 독립 심층 분석은 미완료이며, 이번 결과를 디컴파일 결과라고 부르지 않는다. 현재 REA 대상은 닫힌 상태다.

## 3. 측정 결과

- 대상 EXE SHA-256: `c6709ece5468fadeb7eda085355a7dc5157601ddc589fb2beac6f5726a99f933`.
- scripts.pbo SHA-256: `ff0ad7ee5297c75da74364569a830bc0223315c0131579a0c11a7875d64cb2e2`.
- DebugOverrideAnimationSpeed 등록 핸들러:0x140aeba50. 속도 인자를 전달하는 직접호출:0x140aebad0→0x14017a950.
- 선택한 헬퍼 바이트:c2 00 00, 즉 ret 0 한 명령. 해당 헬퍼에는 속도 저장·시계 갱신·평가 호출이 없다.
- SetAnimFrame은 entity+0x60 검사 뒤 vtable+0xc8로 전달한다. SetAnimSlot도 entity+0x60 검사 후 별도 헬퍼로 전달한다.
- 관련 기존 실행 runtime20261031-093649에서 선택 Animal의1839개 안정된 표본은 해당 컴포넌트가 null이었고 읽힌 하류 경로는0개였다. 이는 과거 B8FB 한 개체·8렌더파트 진단의 결과로, 이번에 B93D를 새 실행한 결과가 아니다.
- 새8/64 실행·p50/p95/p99·영상·GPU 표시 측정:없음.20ms p95는 여전히 목표다.

## 4. 원인 분석

확인:DebugOverrideAnimationSpeed는 이름으로 리소스를 찾은 경우 빈 헬퍼를 호출하고 성공값을 반환하는 분기가 있다. 따라서 true 반환만으로 재생 속도 수정이 됐다고 판단할 수 없다. 현재 EXE의 해당 경로를 시간 손실 해결책으로 채택하지 않는다.

확인:SetAnimFrame/SetAnimSlot의 공통 컴포넌트 가드는 기존 선택 Animal의 generic Animate 조사에서 null이었던 필드다. 두 API를 새 clock 제어 경로로 확정하거나, 같은 조건에서 무작정 실행할 근거는 확보되지 않았다. 다른 엔티티 종류나 모든 런타임에서 이 API가 작동하지 않는다는 결론은 아니다.

미확인:PreAnim_SetFloat는 공개 그래프 변수 설정 경로지만 그 변수의 실제 소비 노드와 평가 주기·위상에 미치는 효과는 별도 문제다. 변수 설정 선언을 임의 clock/residual setter로 해석하지 않는다. 그래프 변수로 속도만 바꿔도 표시 갱신 간격까지 해결된다고 주장하지 않는다.

기존에 확인한 입력dt/고정50ms평가/누적기 overshoot 손실과 메모리 한정 residual 실험은 보존한다. 이번 분석은 그 메커니즘을 수정하거나, CPU 포즈를 최종GPU표시와 동일하다고 증명한 것이 아니다.

## 5. 검증 수준

공개 속도 후보의 빈 헬퍼와 전역 frame/slot 가드 대조:한정된 정적 진단 성공. REA 심층 검증:공급자 형식 제약으로 미완료. API 실호출 효과·8/64 기능 회귀·시각 품질·성능 개선·최종 수용:이번에 수행하지 않았다. 지원되는 배포용 시계 제어는 아직 확보하지 못했다.

## 6. 변경 사항

분석기·API 선정 목록·네이티브 등록/명령·3바이트 출처 증거·MD/TXT만 추가했다. 처음 분석기 이름 inspect.py가 Python 표준 모듈 이름과 충돌해 import가 실패했으며 파일명을 바꾼 뒤 성공했다. 게임 동작에는 영향을 주지 않았다. 게임코드·PBO·운영 서버·공용 패키지·원시 실험·키 변경 없음. 새 PBO·서명 대상 없음.

## 7. 원시 증거

installed-api-scan/native-bindings/native-global-bindings/helper-extraction/speed-helper.bin/rea-raw-unavailable/results와 분석 스크립트를 보존한다. 선택 바이트에는 EXE hash·원래 offset·주소·범위를 연결했다. 과거 generic receiver 체크포인트의 기존 hash도 대조했다. 공개 자료는 선정 선언/함수와 한계이며 전체 게임 파일·로컬 경로·프로세스·비밀정보를 포함하지 않는다. 공개 HTTP 성공과 외부AI 읽기는 분리한다.

## 8. 잔여 문제

다음 애니메이션 실험은 현재 그래프의 실제 변수 소비 노드를 확인하는 데 한정한다. nativeAge 또는 residual을 직접 설정할 지원 경로가 확인되기 전에는 보간·고정배속·duration 오용·강제LOD를 배포하지 않는다. 가능 경로가 나타나면 한 개체에서 실제 포즈·화면·시간을 대조하고, 효과와 비용을 확인한 뒤8→64 회귀로 이어간다. 경로가 없으면 엔진 제약과 필요한 도구를 명시한다.

64성능 비용 귀속과 자연스러운 등반 중 밀침은 별도 통제 실험으로 관리한다. 전체 목표가 애니메이션 공개 API 하나의 부재로 완료되거나 중단된 것은 아니다. 정상 낙하·복구와 사용자 기준을 유지한다.

선정 공개 본문. 로컬 보고서 SHA-256: `3accb1261e7eff4ef45a3d5163625ab9b5697e52c0355206d3c96b02aa39fe64`. 원시 전체가 아니다.
