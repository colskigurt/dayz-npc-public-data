# native 계측 없는 동일 BattlEye 경고 확인과 진단 실행 준비 한계

작성: 2026-11-01T17:08:55.078036+09:00

## 1. 작업 목표

계측 없는 기준 실행의 실제 오류를 확보해 이전 native 계측 시험의 BattlEye 경고와 같은 범주인지 확인한다. 지원되는 진단 실행 경로를 조사하되 기존 NPC 구현·50ms 시계 손실 근거·64개체 비용 실험은 보존한다.

## 2. 실제 수행

일반 client/server를 고정 run·루트 이동 OFF·물리1파트 조건으로1회 실행했다. 네이티브 관찰기는 붙이지 않았다. 서버 disconnect 로그를250ms 간격으로 확인하는 bounded CU를 미리 연결했고 marker 직후와1.2초 뒤의 화면2장을 저장했다. 첫 CU 준비는 include_text/include_screenshot을 모두 끈 잘못된 요청으로 실패해 수정했으며, 수정 후 포착에 성공했다.

공식 [Modding Basics](https://community.bistudio.com/wiki/DayZ:Modding_Basics)의 검색 발췌에서 진단 exe를 이용한 서버 실행과 BattlEye=0 안내를 확인했다. 본문 GET은403이므로 전체 문서를 읽었다고 주장하지 않는다. 설치된 DayZDiag 1.30.164200.56, SHA A500BE804FFED5CDE369337AEB6AB477EF5EE0B5015764BE2B1953C85EC3060D로 별도 서버·client 환경을 준비했다.

진단 환경은3회 모두 NPC 생성 전에 하네스가 멈췄다. 첫 회는 기본 PluginItemDiagnostic의 미등록 PluginConfigDebugProfile 조회 스택을 SCRIPT(E) 중단 조건이 잡았다. 설치 scripts.pbo를 읽어 OnInit의 조회 뒤 null 검사와 Diag/Debug 등록 분기를 확인했다. 두 번째는 해당8줄 스택의 정확 일치만 허용했지만 같은 스택이 여러 로그에 복제된 경우를 처리하지 못했다. 세 번째는 반복 스택도 비교했지만 live gate는 다시 false였다. 종료된 전체 로그24줄은 같은8줄 스택3복사와 정확히 일치한다는 것을 실제 PowerShell 식으로 재현했다. 중단 시점의 정확한 로그 snapshot은 없어 원인을 확정하지 않는다. 준비 스크립트의 로그 선택·개수 assert 실패는 runtime 횟수에 더하지 않았다.

## 3. 측정 결과

일반 실행: 서버 pose 2151행, READY부터 disconnect 32.17초, 서버 DONE보다 12.538초 앞선 이탈이다. 서버는 Game restart required라는 BattlEye kick 사유를 기록했고 두 번째 CU 화면에서0x000400F0, BattlEye, Game restart required를 직접 확인했다. 첫 화면은 아직 월드였으므로 marker 직후 한 장만으로 오류가 없다고 판단하면 놓칠 수 있다. 두 프레임은 시각 품질·FPS 측정이 아니다.

서버 disconnect flags는 authFailed=false, alive=true, health=100, logoutTime=15다. 이번에도 그 값만으로 인증 전반의 정상을 입증하지 않는다. BEClient 모듈을 fresh 조회에서도 확인했다. 일반 실행은 DONE·client 종료 marker 확보, client 강제 종료 없음, SCRIPT(E)0줄, crash log0개다.

진단3회는 모두 NPC READY 없음·DONE 없음이다. 해당 실패 후 소유 서버를 정지하고 client도 닫았으며 일부 client는 강제 종료였다. 유효한 네트워크/애니메이션 측정으로 취급하지 않는다. known-warning-v2의 완료 로그 replay는24/8줄 일치, compile/role-failure 없음, crash0이다. live gate의 false와 불일치가 남는다.

총 runtime 준비/실행4회이며 실제 NPC 실행은 일반 환경1회다. 각 회 기존3개 PBO 해시·서명을 검증했다. 새8/64 회귀, frame-time p50/p95/p99, CPU self-time, GPU/display 시간, 할당/GC는 측정하지 않았다.20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: native attachment가 없는 상태에서도 이전 계측 시험과 같은 오류 코드·문구 및 서버 BattlEye kick 사유가 발생했다. 따라서 이 경고의 발생에 native attachment가 반드시 필요하다는 가설은 반증됐다. 그러나 내부 어떤 조건이 재시작을 요청하는지는 미확인이다. 서버cfg값·직접 실행·DLL 로드만으로 client의 전체 active/inactive 상태를 확정하지 않는다.

확인된 사실: 진단 전용 기본 플러그인 조회 경로는 null 결과를 검사한다. 동일 스택이3종 로그에 반복되어 전체 SCRIPT(E) 줄수24를 만든다. 이 경고를 무시하고 진단 성공이라고 보고하지 않는다. NPC 실패가 아니라 시험 하네스 중단 단계로 분류한다.

가설: 세 번째 live gate와 완료 로그 replay의 차이는 쓰기 중이거나 소스별 갱신이 어긋난 스냅샷 때문일 수 있다. 다만 실제 거부 snapshot이 없어 확정하지 않는다. 이 상태에서 게임 재실행을 반복하기보다 오프라인에서 실제 PowerShell guard와 partial/duplicate/unexpected 입력을 검증해야 한다. Python 모델 검사만으로 실제 PowerShell 실행을 대신하지 않는다.

## 5. 검증 수준

계측 없는 동일 오류 화면·서버 사유 포착은 진단 성공이다. 연결 문제 해결·client 비활성 검증은 미완료다. 진단 exe의 연결 기준 시험은 미실행 수준이다. 전체8/64 회귀, 시각 품질, 성능 목표와 최종 수용은 미완료다.

## 6. 변경 사항

기존 research 하위의 격리 미션·launch·bounded CU 저장·진단 경고 guard만 추가했다. 원래 게임 코드·PBO·ANM·9350 후보·운영 서버는 변경하지 않았다.3개 PBO의 기존 서명·검증은 각 시도에서 성공했다. 새 PBO가 없어 신규 서명 대상은 없다.

서비스·드라이버·영구 보안 설정·anti-cheat 바이너리를 변경하지 않았다. 새 도구 설치·영상·대용량 dump는 없다. 진단 exe의 해시·역할·기본 플러그인 경고는 일반 실행과 구분해 남겼다.

## 7. 원시 증거

분석 ID no-observer-error-20261101-1700. root의 client/server profile·pose·ui-observation·capture-disconnect.js·disconnect-0/1.jpg를 보존한다. 두 번째 화면 SHA 3668e8cc0f348eac2a12e45cce04c840d3ccad8e0edc8d7cbbb495e4207ec140이다. diag-baseline/diag-known-warning/diag-known-warning-v2에는 실패 당시 파일을 유지했다. shipped-diag-plugin-context는 설치 PBO/멤버 해시와 선정 소스이며 diag-guard-complete-replay는 실제 PowerShell 종료 로그 재검사다.

공개본은 이벤트 투영·사유·화면 전사·화면 해시·소스 발췌만 포함한다. 사용자 ID가 들어간 서버 kick 원문·로컬 절대 경로·CLI·프로세스 정보·이미지·전체 원시 로그·게임 자산·키는 제외한다. 원본 해시와 종료·JSON·파일 안정성을 확인한 뒤 기존 보고서 체계에 저장했다.

## 8. 잔여 문제

다음은 live gate의 실제 PowerShell 판정식을 오프라인에서 완결·중복·부분·예상 밖 오류 입력으로 검증하고, 다음1회 bounded 진단 시험이 거부될 때 그 정확한 snapshot과 판정 이유를 저장하도록 만드는 것이다. 미완결 경고를 무기한 허용하거나 일반 오류 검사를 없애지 않는다. 지원 진단 연결이 검증되기 전 native 연결은 재개하지 않는다.

진단 exe는 일반 exe와 다른 빌드이므로 연결이 되더라도 일반 client 성능·시각 수용을 대신하지 않는다. 연결 gate 해결과 오프라인 mode0/표시 진단,64개체 기능/비용은 별도 실험으로 유지한다. 새 보간·시간 배율·충돌 검사 생략은 적용하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `620a63292f7702765f88288e39ca9d0b169741315b35cb055ca33baddce936cf`. 원시 전체가 아니다.
